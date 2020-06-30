#ifndef Queue_hpp
#define Queue_hpp

#include <iostream>
#include <cstdlib>

#include "Event.hpp"

template <typename T>
class Queue
{
public:
    Queue()
    {
        this->front = new T;
        this->front->next = NULL;
        this->rear = this->front;
    }
    ~Queue()
    {
        this->clearQueue();
        delete this->front;
        this->front = NULL;
        this->rear = NULL;
    }
    void clearQueue()
    {
        T *temp_node;
        while (this->front->next)
        {
            temp_node = this->front->next;
            this->front->next = temp_node->next;
            delete temp_node;
        }
        this->front->next = NULL;
        this->rear = this->front;
    }

    // 入队时，传递节点指针，外部数据不应该由此类进行管理，所以将数据拷贝一份
    T *enqueue(T& node)
    {
        //谁申请，谁释放。
        T *new_node = new T;
        *new_node = node;
        this->rear->next = new_node;
        this->rear = new_node;
        return this->front;
    }
    T *dequeue()
    {
        if (!this->front->next)
        {
            return NULL;
        }
        T *temp_node;
        temp_node = this->front->next;
        this->front->next = temp_node->next;
        // 如果队列中只有一个节点，那么记得将队尾节点指针置为头结点
        if (this->rear == temp_node)
        {
            this->rear = this->front;
        }
        //出队时，直接将分配的节点的指针返回了出去，而不是拷贝一份再返回
        return temp_node;
    }

    T *orderEnqueue(Event &event)
    {
        Event* temp = new Event;
         *temp = event;
        if (!this->front->next)
        {
            this->enqueue(*temp);
            return this->front;
        }
        Event *temp_event_list = this->front;

        while (temp_event_list->next &&
               temp_event_list->next->cur_time < event.cur_time)
        {
            temp_event_list = temp_event_list->next;
        }

        temp->next = temp_event_list->next;
        temp_event_list->next = temp;

        return this->front;
    }
    int length()
    {
        T *temp_node;
        temp_node = this->front->next;
        int length = 0;
        while (temp_node)
        {
            temp_node = temp_node->next;
            length++;
        }
        return length;
    }

private:
    T *front;
    T *rear;
};

#endif