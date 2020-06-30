#ifndef Event_hpp
#define Event_hpp
#include "Random.hpp"

const int Max_Time = 100; 

struct Event{
    int cur_time;
    //方便数组来管理
    int event_type;
    Event* next;

    Event(int _cur_time = Random::getRandom(POISSON, 0.5),
        int _event_type = -1):
        cur_time(_cur_time),
        event_type(_event_type),
        next(NULL){}
};

#endif