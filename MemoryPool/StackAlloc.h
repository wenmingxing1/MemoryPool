#ifndef STACKALLOC_H_INCLUDED
#define STACKALLOC_H_INCLUDED

#include<memory>
using namespace std;

template <typename T>
struct StackNode_
{
    T data;
    StackNode_* prev;
};

//
//
template <class T, class Alloc = allocator<T>>
class StackAlloc
{
public:
    //
    typedef StackNode_<T> Node;
    typedef typename Alloc::template rebind<Node>::other allocator;

    //
    StackAlloc() {head_ = 0;}
    //
    ~StackAlloc() {clear();}

    //
    bool empty() {return (head_ == 0);}

    //
    void clear() {
        Node* curr = head_;
        while (curr != 0) {
            Node* tmp = curr->prev;
            allocator_.destroy(curr);
            allocator_.deallocate(curr, 1);
            curr = tmp;
        }
        head_ = 0;
    }

    //ÈëÕ»
    void push(T element) {
        //
        Node* newNode = allocator_.allocate(1);
        //
        allocator_.construct(newNode, Node());

        //
        newNode->data = element;
        newNode->prev = head_;
        head_ = newNode;
    }

    //³öÕ»
    T pop() {
       T result = head_->data;
       Node* tmp = head_->prev;
       allocator_.destroy(head_);
       allocator_.deallocate(head_, 1);
       head_ = tmp;
       return result;
    }

    //
    T top() {return (head_->data);}

private:
    allocator allocator_;
    Node* head_;
};


#endif // STACKALLOC_H_INCLUDED
