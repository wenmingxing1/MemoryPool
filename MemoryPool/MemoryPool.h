#ifndef MEMORYPOOL_H_INCLUDED
#define MEMORYPOOL_H_INCLUDED

#include<climits>
#include<cstddef>

template <typename T, size_t BlockSize = 4096>
class MemoryPool
{
public:
    //
    typedef T* pointer;

    //定义rebind<U>::other接口
    template <typename U> struct rebind {
        typedef MemoryPool<U> other;
    };

    //默认构造，初始化所有的槽指针
    //不抛出异常
    MemoryPool() noexcept {
        currentBlock_ = nullptr;
        currentSlot_ = nullptr;
        lastSlot_ = nullptr;
        freeSlot_ = nullptr;
    }

    //析构函数，销毁一个现有内存池
    ~MemoryPool() noexcept {
        //循环销毁内存池中分配的内存区块
        slot_pointer_ curr = currentBlock_;
        while (curr != nullptr) {
            slot_pointer_ prev = curr->next;
            operator delete(reinterpret_cast<void*>(curr));
            curr = prev;
        }
    }

    //同一时间只能分配一个对象，n和hint会被忽略
    pointer allocate(size_t n = 1, const T* hint = 0) {
        //如果有空闲的对象槽，则直接将空闲区域交付给申请内存程序
        if (freeSlot_ != nullptr) {
            pointer result = reinterpret_cast<pointer>(freeSlot_);
            freeSlot_ = freeSlot_->next;
            return result;
        }

        else {
            //如果当前区块的对象槽不够用了，则分配一个新的内存区块
            if (currentSlot_ >= lastSlot_) {
                //分配一个新的内存区块，并指向一个内存区块
                data_pointer_ newBlock = reinterpret_cast<data_pointer_>(operator new(BlockSize));
                reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
                currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);
                //填补整个区块来满足元素内存区域的对齐要求
                data_pointer_ body = newBlock + sizeof(slot_pointer_);
                uintptr_t result = reinterpret_cast<uintptr_t>(body);
                size_t bodyPadding = (alignof(slot_type_)-result) % alignof(slot_type_);
                currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
                lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + BlockSize - sizeof(slot_type_));
            }
            return reinterpret_cast<pointer>(currentSlot_++);
        }
    }

    //销毁指针p指向的内存区块
    void deallocate(pointer p, size_t n = 1) {
        if (p != nullptr) {
            //reinterpret_cast是强制类型转换符
            //要访问next必须强制将p转换成slot_pointer
            //
            reinterpret_cast<slot_pointer_>(p)->next = freeSlot_;
            freeSlot_ = reinterpret_cast<slot_pointer_>(p);     //设定为freeSlot_
        }
    }

    //调用构造函数，这里使用了C++11的可变参数模板
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new (p) U (std::forward<Args>(args)...);    //  调用新建对象的构造函数
    }

    //销毁内存池中的对象，即调用对象的析构函数
    template <typename U>
    void destroy(U* p)
    {
        p->~U();
    }


private:
    //用于存储内存池中的对象槽
    //要么被实例化为一个存放对象的槽
    //要么被实例化为一个指向存放对象槽的指针
    union Slot_ {
        T element;
        Slot_* next;
    };

    //数据指针
    typedef char* data_pointer_;
    //对象槽
    typedef Slot_ slot_type_;
    //对象槽指针
    typedef Slot_* slot_pointer_;



    //指向当前内存区域
    slot_pointer_ currentBlock_;
    //指向当前内存区块的一个对象槽
    slot_pointer_ currentSlot_;
    //指向当前内存区块的最后一个对象槽
    slot_pointer_ lastSlot_;
    //指向当前内存区块中的空闲对象槽
    slot_pointer_ freeSlot_;

    //检查定义的内存池大小是否过小;static_assert如果第一个表达式为false，则会输出指定错误信息，并编译失败
    static_assert(BlockSize >= 2*sizeof(slot_type_), "BlockSize too small.");
};

#endif // MEMORYPOOL_H_INCLUDED
