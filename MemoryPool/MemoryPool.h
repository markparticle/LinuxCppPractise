#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <climits>
#include <cstddef>
#include <cstdint>
#include <utility>

template <typename T, size_t blockSize = 4096>
class MemoryPool
{
public:
    //定义rebind<U>::other 接口
    //rebind实现了对不同类型使用同一种内存分配策略的要求。
    //一个结构体，允许一个类型的对象的allocator分配其它类型的对象的存储
    template <typename U>
    struct rebind
    {
        typedef MemoryPool<U> other;
    };


    //显式声明此函数不会抛出异常
    MemoryPool() noexcept
    {
        currBlock_ = nullptr;
        currSlot_ = nullptr;
        lastSlot_ = nullptr;
        freeSlots_ = nullptr;
    }
    ~MemoryPool() noexcept
    {
        slot_pointer_ curr = currBlock_;
        while (curr != nullptr)
        {
            slot_pointer_ tmp = curr->next;
            // 转化为 void 指针， void 类型不需要调用析构函数,只释放空间
            operator delete(reinterpret_cast<void *>(curr));
            curr = tmp;
        }
    }

    //分配一个元素的内存空间
    T *allocate(size_t n = 1, const T *hint = 0)
    {
        if (freeSlots_ != nullptr)
        {
            T *res = reinterpret_cast<T *>(freeSlots_);
            freeSlots_ = freeSlots_->next;
            return res;
        }
        if (currSlot_ >= lastSlot_)
        {
            /* 将 data_pointer_ 强转为 slot_pointer_ 并不意味着两者就是同样的东西，
            一个 Block 里面可以包含若干个 Slot，而把一块 Block 内存强行转换到 Slot 内存，
            指针只会去解释一个 Slot大小的内存空间，而后面的空间并没有被使用，
            所以 data_pointer_ 并非多余，data_pointer_ 的任务是去申请一块 Block, 
            而之后使用这块内存是通过强转为 slot_pointer_ 来进行的，因此非常有必要的。
            
            在一个 Block 中，第一个 Slot 会被解释为一个 slot_pointer_，并指向前一个申请的 Block。
            而一个指针占用的内存空间和一个模板参数所占用的空间并不相同，
            所以要求得一个 Block 内的第一个 Slot，所以需要额外将整个区块进行填补，
            满足内存对齐（所以 body 是由 newBlock 与 sizeof(slot_pointer_) 而来，
            而非 sizeof(slot_type_)）。
            */

            //申请一个新的内存块，放入内存池
            // operator new 申请blockSize大小内存，返回 void* 指
            data_pointer_ newBlock = reinterpret_cast<data_pointer_>(operator new(blockSize));
            // 原来的 block 链头接到 newblock
            reinterpret_cast<slot_pointer_>(newBlock)->next = currBlock_;
            currBlock_ = reinterpret_cast<slot_pointer_>(newBlock);

            //计算为了对齐应该空出多少位置 
            data_pointer_ body = newBlock + sizeof(slot_pointer_);
            
            uintptr_t body_pointer_size = reinterpret_cast<uintptr_t>(body);
            // 内存对齐 alignof内存操作符 slot 结构体大小  body已经包含了body_pointer指针所以要减去
            size_t bodyPadding = (alignof(slot_type_) - body_pointer_size) % alignof(slot_type_);
            //bodyPadding并且大部分情况下计算结果均为 0
            // currentslot_ 为 block 开始的地方加上 bodypadding 个 char* 空间
            currSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
            // 计算最后一个能放置 slot_type_ 的位置
            lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + blockSize - sizeof(slot_type_));
        }
        return reinterpret_cast<T *>(currSlot_++);
    }

    template <typename U>
    void destroy(U *p)
    {
        //调用对象的析构函数
        p->~U();
    }
    void deallocate(T *p, size_t n = 1)
    {
        if (p != nullptr)
        {
            //强类型转换符
            reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
            freeSlots_ = reinterpret_cast<slot_pointer_>(p);
        }
    }

    /*
    可变参数模板
    typename... Args is called a template parameter pack, 
    可变参数函数模板
    Args... args is called a function parameter pack
    */
    template <typename U, typename... Args>
    void construct(U *ptr, Args &&... args)
    {
        //将参数解包
        new (ptr) U(std::forward<Args>(args)...);
    }

private:
    union Slot_ {
        T element;
        Slot_ *next;
    };
    typedef char *data_pointer_;
    typedef Slot_ slot_type_;
    typedef Slot_ *slot_pointer_;

    //当前内存块的指针
    slot_pointer_ currBlock_;
    //当前内存块中的对象槽
    slot_pointer_ currSlot_;
    slot_pointer_ lastSlot_;
    slot_pointer_ freeSlots_;

    static_assert(blockSize >= 2 * sizeof(slot_type_), "BlockSize too small.");
};

#endif