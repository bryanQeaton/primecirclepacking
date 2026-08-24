#ifndef PRIMECIRCLEPACKING_PROGRESSIVEPACK_H
#define PRIMECIRCLEPACKING_PROGRESSIVEPACK_H
#include "pack.h"


constexpr double primes[100] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
467, 479, 487, 491, 499, 503, 509, 521, 523, 541
};

inline bool progressivePack(Pack pack,const int n) {
    if (n<=1) {
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
            pack.inners.push_back(c);
            auto res=progressivePack(pack,n-1);
            pack.inners.pop_back();
            if (res){return true;}
        }
    }
    return false;
}




#endif //PRIMECIRCLEPACKING_PROGRESSIVEPACK_H
