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

    //����rebind<U>::other�ӿ�
    template <typename U> struct rebind {
        typedef MemoryPool<U> other;
    };

    //Ĭ�Ϲ��죬��ʼ�����еĲ�ָ��
    //���׳��쳣
    MemoryPool() noexcept {
        currentBlock_ = nullptr;
        currentSlot_ = nullptr;
        lastSlot_ = nullptr;
        freeSlot_ = nullptr;
    }

    //��������������һ�������ڴ��
    ~MemoryPool() noexcept {
        //ѭ�������ڴ���з�����ڴ�����
        slot_pointer_ curr = currentBlock_;
        while (curr != nullptr) {
            slot_pointer_ prev = curr->next;
            operator delete(reinterpret_cast<void*>(curr));
            curr = prev;
        }
    }

    //ͬһʱ��ֻ�ܷ���һ������n��hint�ᱻ����
    pointer allocate(size_t n = 1, const T* hint = 0) {
        //����п��еĶ���ۣ���ֱ�ӽ��������򽻸��������ڴ����
        if (freeSlot_ != nullptr) {
            pointer result = reinterpret_cast<pointer>(freeSlot_);
            freeSlot_ = freeSlot_->next;
            return result;
        }

        else {
            //�����ǰ����Ķ���۲������ˣ������һ���µ��ڴ�����
            if (currentSlot_ >= lastSlot_) {
                //����һ���µ��ڴ����飬��ָ��һ���ڴ�����
                data_pointer_ newBlock = reinterpret_cast<data_pointer_>(operator new(BlockSize));
                reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
                currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);
                //���������������Ԫ���ڴ�����Ķ���Ҫ��
                data_pointer_ body = newBlock + sizeof(slot_pointer_);
                uintptr_t result = reinterpret_cast<uintptr_t>(body);
                size_t bodyPadding = (alignof(slot_type_)-result) % alignof(slot_type_);
                currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
                lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + BlockSize - sizeof(slot_type_));
            }
            return reinterpret_cast<pointer>(currentSlot_++);
        }
    }

    //����ָ��pָ����ڴ�����
    void deallocate(pointer p, size_t n = 1) {
        if (p != nullptr) {
            //reinterpret_cast��ǿ������ת����
            //Ҫ����next����ǿ�ƽ�pת����slot_pointer
            //
            reinterpret_cast<slot_pointer_>(p)->next = freeSlot_;
            freeSlot_ = reinterpret_cast<slot_pointer_>(p);     //�趨ΪfreeSlot_
        }
    }

    //���ù��캯��������ʹ����C++11�Ŀɱ����ģ��
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new (p) U (std::forward<Args>(args)...);    //  �����½�����Ĺ��캯��
    }

    //�����ڴ���еĶ��󣬼����ö������������
    template <typename U>
    void destroy(U* p)
    {
        p->~U();
    }


private:
    //���ڴ洢�ڴ���еĶ����
    //Ҫô��ʵ����Ϊһ����Ŷ���Ĳ�
    //Ҫô��ʵ����Ϊһ��ָ���Ŷ���۵�ָ��
    union Slot_ {
        T element;
        Slot_* next;
    };

    //����ָ��
    typedef char* data_pointer_;
    //�����
    typedef Slot_ slot_type_;
    //�����ָ��
    typedef Slot_* slot_pointer_;



    //ָ��ǰ�ڴ�����
    slot_pointer_ currentBlock_;
    //ָ��ǰ�ڴ������һ�������
    slot_pointer_ currentSlot_;
    //ָ��ǰ�ڴ���������һ�������
    slot_pointer_ lastSlot_;
    //ָ��ǰ�ڴ������еĿ��ж����
    slot_pointer_ freeSlot_;

    //��鶨����ڴ�ش�С�Ƿ��С;static_assert�����һ�����ʽΪfalse��������ָ��������Ϣ��������ʧ��
    static_assert(BlockSize >= 2*sizeof(slot_type_), "BlockSize too small.");
};

#endif // MEMORYPOOL_H_INCLUDED
