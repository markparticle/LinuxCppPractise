#ifndef ServiceWindow_hpp
#define ServiceWindow_hpp

#include "Node.hpp"

enum WindowStatus{
    SERVICE,
    IDLE,
};

class ServiceWindow{
public:
    //初始化为闲
    inline ServiceWindow(){
        window_status = IDLE;
    }
    inline bool isIdle() const{
        return window_status == IDLE;
    }
    inline void serveCustomer(Customer &customer){
        this->customer = customer;
    }
    inline void setBusy(){
        window_status = SERVICE;
    }
    inline void setIdle(){
        window_status = IDLE;
    }
    inline int getCusArriveTime() const{
        return customer.arrive_time;
    }
    inline int getCusDuration() const {
        return customer.duration;
    }

private:
    Customer customer;
    WindowStatus window_status;
};


#endif