#ifndef PRIMECIRCLEPACKING_PRIMES_H
#define PRIMECIRCLEPACKING_PRIMES_H
#include <cmath>
#include <cstdint>
#include <vector>

//returns primes up to n
inline std::vector<uint64_t> get_primes(const uint64_t n) {
    const int max_val=static_cast<int>(sqrt(static_cast<double>(n)));
    std::vector is_prime(n,true);
    is_prime[0]=false;is_prime[1]=false;
    for (int i=2;i<=max_val;i++) {
        for (int m=i;m<n;m+=i) {
            if (m==i){continue;}
            is_prime[m]=false;
        }
    }
    std::vector<uint64_t> primes;
    for (int i=0;i<=n;i++) {
        if (is_prime[i]){primes.push_back(i);}
    }
    return primes;
}

#endif //PRIMECIRCLEPACKING_PRIMES_H
