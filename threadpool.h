#ifndef THREADPOOL
#define THREADPOOL

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>

class FixedThreadPool
{
public:
    //explicit关键字的作用就是防止类构造函数的隐式自动转换.
    explicit FixedThreadPool(size_t thread_count)
        : _data(std::make_shared<Data>()) {
        for (size_t i = 0; i < thread_count; i++) {
            //匿名函数 拷贝智能指针，不是拷贝指向的对象
            std::thread([data = _data] {
                std::unique_lock <std::mutex> locker(data->_mtx);
                while (true) {
                    if (!data->_tasks.empty()) {
                        //如果有任务，取出任务执行
                        auto current = std::move(data->_tasks.front());
                        data->_tasks.pop();
                        locker.unlock();
                        //执行任务，处于游离状态
                        current();
                        locker.lock();
                    }
                    else if (data->_is_shutdown)
                        break;
                    else
                        data->_cond.wait(locker); //等待任务，处于游离状态
                }
            }).detach();//分离该线程（结束时，自动清除。不能调用join进行同步）
        }
    }
    //编译器将为显式声明的 "=default"函数自动生成函数体。 比用户自定义的默认构造函数效率更高
    //"=default"函数特性仅适用于类的特殊成员函数，且该特殊成员函数没有默认参数。
    FixedThreadPool() = default;
    // 默认移动构造函数 使用右值的时候
    FixedThreadPool(FixedThreadPool&&) = default;
    ~FixedThreadPool(){
        //如果还有线程执行，
        if(static_cast<bool>(_data)){
            //阻塞进程，直到所有通过其创建的线程均已执行完毕。
            {
                std::lock_guard<std::mutex> locker(_data->_mtx);            
                _data->_is_shutdown = true;
            }
            //唤醒所有线程
            _data->_cond.notify_all();
        }
    }
    //右值引用
    template <class F> 
    void execute(F&& task){
        {
            std::lock_guard<std::mutex> locker(_data->_mtx);
            //插入该任务
            //std::forward只有在它的参数绑定到一个右值上的时候，它才转换它的参数到一个右值。
            _data->_tasks.emplace(std::forward<F>(task));
        }
        //有任务进来，唤醒一个线程
        _data->_cond.notify_one();
    }

    //callback接口
    template <class F, class... Args>
	void execute2(F&& func, Args&&... args) {
		{
			auto  task = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
			std::lock_guard<std::mutex> lk(_data->_mtx);
			_data->_tasks.emplace(std::forward<decltype(task)>(task));
		}
		_data->_cond.notify_one();
	}

private:
    struct Data
    {
        std::mutex _mtx;
        std::condition_variable _cond;
        bool _is_shutdown = false;
        //通过std::function对C++中各种可调用实体
        //（普通函数、Lambda表达式、函数指针、以及其它函数对象等）的封装，形成一个新的可调用的std::function对象
        std::queue<std::function<void()>> _tasks;
    };
    //智能指针  一个线程池实例对应的实际的 data 对象只有一个
    std::shared_ptr<Data> _data;
};

#endif