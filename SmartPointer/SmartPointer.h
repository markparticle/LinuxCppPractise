/*
 * @Author       : mark
 * @Date         : 2020-05-23
 * @copyleft GPL 2.0
 */ 

template <typename T>
class SmartPointer {
public:
    SmartPointer(): pointer_(nullptr) {};
    SmartPointer(void *pointer): pointer_(pointer) {
        if(pointer_) pointer_->incRefCount();
    };
    ~SmartPointer() {
        //不能指向无法被delete释放的内存资源
        if(pointer_ && pointer_->decRefCount() == 0) delete pointer_;
    };
    //拷贝构造函数
    SmartPointer(const SmartPointer &other): pointer_(other.pointer_) {
        //引用计数+1
        if(pointer_) pointer_->incRefCount();
    };
    //重载赋值操作符
    SmartPointer &operator = (const SmartPointer &other) {
        T *temp(other.pointer_);
        //新指向对象，引用计数+1
        if(temp) temp->incRefCount();

        //原指向对象，引用计数-1，如果为0，销毁原资源对象
        if(pointer_ && pointer_->decRefCount() == 0) delete pointer_;

        //智能指针指向新的资源对象
        pointer_ = temp;
        return *this;
    }
    T& operator* () const {
        return *pointer_;
    };
    T* operator-> () const {
        return pointer_;
    }
    
    bool operator== (const T* o)const {
        return pointer_ == o;
    }
    bool operator== (const SmartPointer& o) const {
        return pointer_ == o.pointer_;
    }
    bool operator!= (const T* o)const {
        return pointer_ != o;
    }
    bool operator!= (const SmartPointer& o) const {
        return pointer_ != o.pointer_;
    }

private:
    T *pointer_; //指向智能指针实际对应的内存资源，根据参数自动推到规则，定义内部资源指针类型
};

//需要继承Refbase类的对象
class RefBase
{
public:
    RefBase(): count_(0) {};
    void incRefCount() {
        count_++;
    }
    int decRefCount() {
        count_--;
        return count;
    }
    int getRefCount() {
        return count_;
    }
    virtual ~RefBase() {};
private:
    int count_;
}