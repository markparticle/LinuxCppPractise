#include <iostream>
#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <unistd.h>
#include "threadpool.h"
using namespace std;

// struct Repository{
//     int Buff[BuffSize];
//     size_t read_pos;
//     size_t write_pos;
//     std::mutex mtx;
//     std::condition_variable repo_not_full;
//     std::condition_variable repo_not_empty;
// };

queue<int> Q;
mutex condition_mutex;

condition_variable cond_consumer;
condition_variable cond_producer;

int buff_size;
int producer_time = 1;
int consumer_time = 2;
int MaxProduct = 50;

void producer()
{
    while (true)
    {   
        int data = 0;
        sleep(producer_time);
        {
            unique_lock<mutex> locker(condition_mutex);
            //如果满了，阻塞当前线程
            while (Q.size() == buff_size)
            {
                cout << "仓库满了，等待消费者购买!\n";
                cond_producer.wait(locker);
            }
            //开始生产
            if(MaxProduct == 0) {
                locker.unlock();
                break;
            }
            data = MaxProduct;
            Q.push(MaxProduct);
            MaxProduct--;
            printf("生产者%d：生成产品ID：%d 总商品数%d件\n", this_thread::get_id(), data, Q.size());
            locker.unlock();
        }
        cond_consumer.notify_all();
    }
    cout << "生产者 Done!\n";
}

void consumer()
{
    while (true)
    {   
        int data = 0;
        sleep(consumer_time);
        {
            unique_lock<mutex> locker(condition_mutex);
            while (Q.empty() && MaxProduct > 0)
            {   
                printf("仓库无产品，等待商品生产!\n");
                //消费者等待商品
                cond_consumer.wait(locker);
            }
            if(Q.empty() && MaxProduct == 0){
                locker.unlock();
                break;
            }
            data = Q.front();
            Q.pop();
            printf("消费者%d：买了商品ID：%d 剩余%d件\n", this_thread::get_id(), data, Q.size());
            locker.unlock();
        }
        cond_producer.notify_all();
    }
    cout << "消费者 Done!\n";
}

int main()
{
    std::shared_ptr<FixedThreadPool> threadpool;
    buff_size = 10;
    int n = 6;
    threadpool = std::make_shared<FixedThreadPool>(8);

    for(int i = 0; i < n; i++){
        threadpool->execute2(producer);
        threadpool->execute2(consumer);
    }
    getchar();
    return 0;
}