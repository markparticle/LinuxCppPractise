#ifndef Node_hpp
#define Node_hpp

#include "Random.hpp"
const int Max_Duration = 1; 
const int Min_Duration = 100; 
struct Node{
    int arrive_time;
    int duration;
    struct Node *next;

    Node(int _arrive_time = 0,
        int _duration = Random::getRandom(EXPONENTAIL, 0.1)):
        arrive_time(_arrive_time), 
        duration(_duration), 
        next(NULL){}
};
typedef struct Node Customer;

#endif