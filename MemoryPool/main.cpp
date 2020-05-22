#include <iostream>
#include <cassert>
#include <ctime>
#include <vector>

#include "StackAlloc.h"
#include "MemoryPool.h"

const int kELEMS = 100000;
const int kREPS = 100;

int main()
{
    clock_t start;

    StackAlloc<int, std::allocator<int>> stackDefaule;
    start = clock();
    for (int j = 0; j < kREPS; j++)
    {
        assert(stackDefaule.empty());
        for (int i = 0; i < kELEMS; i++)
            stackDefaule.push(i);
        for (int i = 0; i < kELEMS; i++)
            stackDefaule.pop();
    }
    std::cout << "Default Allocator Time: ";
    std::cout << ((static_cast<double>(clock()) - start) / CLOCKS_PER_SEC) << "\n\n";
    
    // 使用内存池
    StackAlloc<int, MemoryPool<int> > stackPool;
    start = clock();
    for (int j = 0; j < kREPS; j++) {
        assert(stackPool.empty());
        for (int i = 0; i < kELEMS; i++)
          stackPool.push(i);
        for (int i = 0; i < kELEMS; i++)
          stackPool.pop();
    }
    std::cout << "MemoryPool Allocator Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";


    std::vector<int> stackVector;
    start = clock();
    for (int j = 0; j < kREPS; j++)
    {
        assert(stackVector.empty());
        for (int i = 0; i < kELEMS; i++)
            stackVector.push_back(i);
        for (int i = 0; i < kELEMS; i++)
            stackVector.pop_back();
    }
    std::cout << "Vector Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";

    return 0;
}