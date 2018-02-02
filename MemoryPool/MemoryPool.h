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
    }

    //Ĭ�Ϲ��죬��ʼ�����еĲ�ָ��
    //���׳��쳣
    MemoryPool() noexcept {
        currentBlock_ = nullptr;
        currentSlot_ = nullptr;
        lastSlot_ = nullptr;
        freeSlot_ = nullptr;
    }

    //��������������һ�������ڴ��
    ~MemoryPool() noexcept;

    //ͬһʱ��ֻ�ܷ���һ������n��hint�ᱻ����
    pointer allocate(size_t n = 1, const T* hint = 0);

    //����ָ��pָ����ڴ�����
    void deallocate(pointer p, size_t n = 1);

    //���ù��캯��������ʹ����C++11�Ŀɱ����ģ��
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args);

    //�����ڴ���еĶ��󣬼����ö������������
    template <typename U>
    void destroy(U* p)
    {
        P->~U();
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
    static_assert(BlockSize >= 2*sizeof(solt_type_), "BlockSize too small.");
};

#endif // MEMORYPOOL_H_INCLUDED
