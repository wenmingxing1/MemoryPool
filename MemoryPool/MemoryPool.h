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
    }

    //默认构造，初始化所有的槽指针
    //不抛出异常
    MemoryPool() noexcept {
        currentBlock_ = nullptr;
        currentSlot_ = nullptr;
        lastSlot_ = nullptr;
        freeSlot_ = nullptr;
    }

    //析构函数，销毁一个现有内存池
    ~MemoryPool() noexcept;

    //同一时间只能分配一个对象，n和hint会被忽略
    pointer allocate(size_t n = 1, const T* hint = 0);

    //销毁指针p指向的内存区块
    void deallocate(pointer p, size_t n = 1);

    //调用构造函数，这里使用了C++11的可变参数模板
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args);

    //销毁内存池中的对象，即调用对象的析构函数
    template <typename U>
    void destroy(U* p)
    {
        P->~U();
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
    static_assert(BlockSize >= 2*sizeof(solt_type_), "BlockSize too small.");
};

#endif // MEMORYPOOL_H_INCLUDED
