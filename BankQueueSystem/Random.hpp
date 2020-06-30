#ifndef Random_hpp
#define Random_hpp

#include <cstdlib>
#include <cmath>

enum RandomType{
    UNIFORM,
    EXPONENTAIL,
    POISSON,
};

class Random{
public:
    static double getRandom(RandomType type, double parameter){
        switch(type){
            case UNIFORM:
                return Uniform(parameter);
            case EXPONENTAIL:
                return Exponentail(parameter);
            case POISSON:
                return Possion(parameter);
            default:
                return 0;
        }
    }

    static double Uniform(double R = 1){
        //[0, 1) 直接服从均匀分布的随机值
        double L = 0;
        return L + (R - L) * std::rand() / (RAND_MAX + 1.0);
    }

    static double Exponentail(double lambda){
        return -log(1 - Uniform()) / lambda;
    }

    static double Possion(double lambda){
        double t = 0.0;
        double p = exp(-lambda);
        double f = p;
        double u = Uniform();
        while(f < u){
            t =  t + 1;
            p = p * lambda / t;
            f += p;
        }
        return t;
    }
};

#endif