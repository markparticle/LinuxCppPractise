#ifndef STACKALLOC_H
#define STACKALLOC_H

#include <memory>

template <typename T>
struct StackNode_
{
    T data;
    StackNode_ *prev;
};

template <typename T, typename Alloc = std::allocator<T>>
class StackAlloc
{
public:
    typedef StackNode_<T> Node;
    typedef typename Alloc::template rebind<Node>::other Allocator;

    StackAlloc() { head_ = nullptr; }

    ~StackAlloc() { clear(); }

    bool empty() const { return head_ == nullptr; }

    T top() const { return head_->data; }

    T pop()
    {
        T topData = head_->data;
        Node *tmp = head_->prev;
        allocator_.destroy(head_);
        allocator_.deallocate(head_, 1);
        head_ = tmp;
        return topData;
    }

    void push(T element)
    {
        Node *newNode = allocator_.allocate(1);
        allocator_.construct(newNode, Node());

        newNode->data = element;
        newNode->prev = head_;
        head_ = newNode;
    }

    void clear()
    {
        Node *curr = head_;
        while (curr)
        {
            Node *tmp = curr->prev;
            //先析构，再回收内存
            allocator_.destroy(curr);
            allocator_.deallocate(curr, 1);
            curr = tmp;
        }
        head_ = nullptr;
    }

private:
    Allocator allocator_;
    Node *head_;
};

#endif