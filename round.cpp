
#include <cstdlib>
#include <iostream>
#include <ctime>
using namespace std;

void Randomize(){
    srand((double)time(NULL));//获取当前时间
}

double get_random(double L, double R){
    if(L > R){
        printf("GenerateRandomNumber: Make sure L <= R.\n");
        exit(1);
    }
    // [L, R]
    //RAND_MAX + 1.0 将RAND_MAX转为double防止溢出
    return L + (R - L + 1) *  rand() /(RAND_MAX + 1.0);
}


int main(){
    Randomize();
    int n = 10000;
    for(int i = 0; i < n; i++){
        printf("%d\n", (int)get_random(0, 10));
    }
    return 0;
}
