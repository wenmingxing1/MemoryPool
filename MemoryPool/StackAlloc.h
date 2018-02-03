#ifndef STACKALLOC_H_INCLUDED
#define STACKALLOC_H_INCLUDED

#include<memory>
using namespace std;

//定义链表栈节点
template <typename T>
struct StackNode_
{
    T data;
    StackNode_* prev;
};

//T为存储的对象类型，Alloc为使用的分配器，并默认使用allocator作为对象的分配器
template <typename T, typename Alloc = allocator<T>>
class StackAlloc
{
public:
    //使用typedef简化类型名
    typedef StackNode_<T> Node;
    typedef typename Alloc::template rebind<Node>::other allocatorOther;

    //默认构造函数
    StackAlloc() {head_ = 0;}
    //析构函数
    ~StackAlloc() {clear();}

    //判断是否为空
    bool empty() {return (head_ == 0);}

    //释放栈中元素的所有内存
    void clear() {
        Node* curr = head_;
        //依次出栈
        while (curr != 0) {
            Node* tmp = curr->prev;
            allocator_.destroy(curr);
            allocator_.deallocate(curr, 1);
            curr = tmp;
        }
        head_ = 0;
    }

    //入栈
    void push(T element) {
        //为一个节点分配内存
        Node* newNode = allocator_.allocate(1);       //分配原始内存
        //调用节点的构造函数
        allocator_.construct(newNode, Node());

        //入栈操作
        newNode->data = element;
        newNode->prev = head_;
        head_ = newNode;
    }

    //出栈
    T pop() {
       T result = head_->data;
       Node* tmp = head_->prev;
       allocator_.destroy(head_);   //调用allocator的析构函数，销魂内存上的内容
       allocator_.deallocate(head_, 1);     //释放内存
       head_ = tmp;
       return result;
    }

    //返回栈顶元素
    T top() {return (head_->data);}

private:
    allocatorOther allocator_;
    //栈顶
    Node* head_;
};


#endif // STACKALLOC_H_INCLUDED
