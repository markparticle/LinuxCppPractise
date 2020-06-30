#include <iostream>
#include <cstdlib>
#include <ctime>
#include "QueueSystem.hpp"

int main(){
    std::srand((unsigned)std::time(0));

    int total_service_time = 240;
    int window_num = 4;
    int simulate_num = 10000;

    QueueSystem system(total_service_time, window_num);
    system.simulate(simulate_num);

    std::cout << "All customer stay in bank: "
        << system.getTotal_customer_num() << "\n";
    std::cout << "The average time of customer stay in bank: "
        << system.getAvgStayTime() << "\n";
        std::cout << "The average time of customer wait in bank: "
        << system.getAvgWaitTime() << "\n";
    std::cout << "The number of customer arrive bank per minute:"
        << system.getAvgCustomers() << "\n";
    return 0;
}