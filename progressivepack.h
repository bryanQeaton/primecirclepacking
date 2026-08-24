#ifndef PRIMECIRCLEPACKING_PROGRESSIVEPACK_H
#define PRIMECIRCLEPACKING_PROGRESSIVEPACK_H
#include "pack.h"


constexpr double primes[15] = {
    2.0, 3.0, 5.0, 7.0, 11.0,
    13.0, 17.0, 19.0, 23.0, 29.0,
    31.0, 37.0, 41.0, 43.0, 47.0
};

inline bool progressivePack(Pack pack,const int n) {
    if (n<2) {
        std::cout<<"eureka!";
        for (auto &n:pack.inners) {
            std::cout<<n.x<<" "<<n.y<<" "<<n.r<<"\n";
        }
        pack.view("test.bmp");
        return true;
    }
    auto potentials=pack.potentials(primes[n-2]);
    for (auto &c:potentials) {
        bool intersecting=false;
        for (int m=0;m<pack.inners.size();m++) {
            if (isIntersecting(c,pack.inners[m])) {
                intersecting=true;
                break;
            }
        }
        if (!intersecting) {
            std::cout<<"test";
            pack.inners.push_back(c);
            auto res=progressivePack(pack,n-1);
            if (res){return true;}
        }
    }
    return false;
}




#endif //PRIMECIRCLEPACKING_PROGRESSIVEPACK_H
