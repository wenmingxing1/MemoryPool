#ifndef STACKALLOC_H_INCLUDED
#define STACKALLOC_H_INCLUDED

#include<memory>
using namespace std;

//��������ջ�ڵ�
template <typename T>
struct StackNode_
{
    T data;
    StackNode_* prev;
};

//TΪ�洢�Ķ������ͣ�AllocΪʹ�õķ���������Ĭ��ʹ��allocator��Ϊ����ķ�����
template <typename T, typename Alloc = allocator<T>>
class StackAlloc
{
public:
    //ʹ��typedef��������
    typedef StackNode_<T> Node;
    typedef typename Alloc::template rebind<Node>::other allocatorOther;

    //Ĭ�Ϲ��캯��
    StackAlloc() {head_ = 0;}
    //��������
    ~StackAlloc() {clear();}

    //�ж��Ƿ�Ϊ��
    bool empty() {return (head_ == 0);}

    //�ͷ�ջ��Ԫ�ص������ڴ�
    void clear() {
        Node* curr = head_;
        //���γ�ջ
        while (curr != 0) {
            Node* tmp = curr->prev;
            allocator_.destroy(curr);
            allocator_.deallocate(curr, 1);
            curr = tmp;
        }
        head_ = 0;
    }

    //��ջ
    void push(T element) {
        //Ϊһ���ڵ�����ڴ�
        Node* newNode = allocator_.allocate(1);       //����ԭʼ�ڴ�
        //���ýڵ�Ĺ��캯��
        allocator_.construct(newNode, Node());

        //��ջ����
        newNode->data = element;
        newNode->prev = head_;
        head_ = newNode;
    }

    //��ջ
    T pop() {
       T result = head_->data;
       Node* tmp = head_->prev;
       allocator_.destroy(head_);   //����allocator�����������������ڴ��ϵ�����
       allocator_.deallocate(head_, 1);     //�ͷ��ڴ�
       head_ = tmp;
       return result;
    }

    //����ջ��Ԫ��
    T top() {return (head_->data);}

private:
    allocatorOther allocator_;
    //ջ��
    Node* head_;
};


#endif // STACKALLOC_H_INCLUDED
