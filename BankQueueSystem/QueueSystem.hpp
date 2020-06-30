#ifndef QueueSystem_hpp
#define QueueSystem_hpp

#include "Event.hpp"
#include "Queue.hpp"
#include "ServiceWindow.hpp"
#include <iostream>
#include <algorithm>

class QueueSystem{
public:

    QueueSystem(int _end_time, int _window_num);
    ~QueueSystem();

    void simulate(int simulate_num);

    inline double getAvgStayTime() const{
        return avg_stay_time;
    }
    inline double getAvgCustomers() const{
        return avg_customers;
    }
    inline double getAvgWaitTime() const{
        return avg_wait_time;
    }
    inline double getTotal_customer_num() const{
        return total_customer_num;
    }


private:
    void run();
    void init();
    void end();
    int getIdleServiceWindow();
    void customerArrived();
    void customerDeparture();

    int window_num;
    int start_time;
    int end_time;
    int total_customer_stay_time;
    int total_customer_num;
    int total_customer_wait_time;

    ServiceWindow* windows;
    Queue<Customer> customer_list;
    Queue<Event> event_list;
    Event* current_event;

    double avg_customers;
    double avg_stay_time;
    double avg_wait_time;
};



QueueSystem::QueueSystem(int _end_time, int _window_num) : end_time(_end_time),
                                                           window_num(_window_num),
                                                           total_customer_stay_time(0),
                                                           total_customer_num(0),
                                                           total_customer_wait_time(0)
{
    //创建服务窗口
    this->windows = new ServiceWindow[window_num];
}

QueueSystem::~QueueSystem()
{
    delete[] windows;
}

void QueueSystem::simulate(int simulate_num)
{
    double sum = 0;
    //模拟
    for (int i = 0; i < simulate_num; i++)
    {
        run();
    }

    //计算平均逗留时间
    avg_stay_time = static_cast<double>(total_customer_stay_time) / total_customer_num;
    avg_wait_time = static_cast<double>(total_customer_wait_time) / total_customer_num;
    //计算每分钟平均顾客时间
    avg_customers = static_cast<double>(total_customer_num) /
                    (end_time * simulate_num);
}

void QueueSystem::init()
{
    //新建到达事件
    current_event = new Event;
}

//开始模拟
void QueueSystem::run()
{
    this->init();
    while (current_event)
    {
        //判断当前事件
        if (current_event->event_type == -1)
        {
            //顾客到达
            customerArrived();
        }
        else
            customerDeparture();
        delete current_event;
        current_event = event_list.dequeue();
    }
    this->end();
}

//结束模拟
void QueueSystem::end()
{
    //所有窗口空闲
    for (int i = 0; i < window_num; i++)
    {
        windows[i].setIdle();
    }
    //队列清空
    customer_list.clearQueue();
    event_list.clearQueue();
}

int QueueSystem::getIdleServiceWindow()
{
    for (int i = 0; i < window_num; i++)
    {
        if (windows[i].isIdle())
        {
            return i;
        }
    }
    return -1;
}

/*目前我们设计的系统是由事件来驱动的，而所有到达事件的创建，
是在当前有顾客到达后，才会得到下一个顾客的到达时间。*/
void QueueSystem::customerArrived()
{
    // 生成下一个到达客户，创建到达事件
    // 当前客户加入顾客排队队伍
    // 找出空闲窗口，如果有空闲窗口则开始服务，创建离开事件

    total_customer_num++;

    //生成下一个顾客
    int time = current_event->cur_time + Random::getRandom(POISSON, 0.5);
    Event temp_event(time);
    //如果下个顾客到达时间小于关门时间，加入事件队列
    if (time < end_time)
    {
        event_list.orderEnqueue(temp_event);
    }

    //当前客户加入客户队列
    Customer *customer = new Customer(current_event->cur_time);
    customer_list.enqueue(*customer);

    //如果有窗口空闲，那么队列第一个进入服务窗口
    int idleIndex = getIdleServiceWindow();
    if (idleIndex >= 0)
    {   
        //从客户队伍中取出，设置窗口服务的顾客及状态
        customer = customer_list.dequeue();
        windows[idleIndex].serveCustomer(*customer);
        windows[idleIndex].setBusy();

        //插入事件队列
        //离开事件的发生时间 = 当前时间事件的发生时间 + 服务时间
        Event temp_event(current_event->cur_time +
                             customer->duration,
                         idleIndex);
        event_list.orderEnqueue(temp_event);
    }
    delete customer;
}

void QueueSystem::customerDeparture()
{
    // 当前窗口服务结束，检查队伍是否有人，若有则该窗口开始服务，加入离开事件

    if (current_event->cur_time < end_time)    //如果银行未关门
    {
        //顾客在银行中的逗留时间 = 当前顾客离开时间 - 顾客的到达时间
        total_customer_stay_time += current_event->cur_time -
            windows[current_event->event_type].getCusArriveTime();
        //如果队伍中有人等待，则立即服务等待顾客
        if (customer_list.length())
        {
            //从客户队伍中取出，设置窗口服务的顾客及状态
            Customer *customer = customer_list.dequeue();
            total_customer_wait_time += current_event->cur_time - customer->arrive_time;
            windows[current_event->event_type].serveCustomer(*customer);
            //插入离开事件
            Event temp_event(
                current_event->cur_time + customer->duration,
                current_event->event_type);
            event_list.orderEnqueue(temp_event);
            delete customer;
        }
        else
        {
            //顾客队伍为空，该窗口设为空闲
            windows[current_event->event_type].setIdle();
        }
    }
}

#endif