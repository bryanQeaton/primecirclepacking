#include <cmath>
#include <iostream>
#include <random>

#include "pack.h"
#include "primes.h"
#include "progressivepack.h"
#include "randompack.h"
/*
 *goal:
 *find the smallest circle in which smaller circles of radius' equalling the sequential prime numbers fit
 * 2 3 5 7 = n=4
 *
 *
 */
constexpr double PI=3.1415926535897932384626433832795028841971;
double area(const double r) {return r*r*PI;}

void computeBoundary() {
    auto primes=get_primes(31);
    int n=primes.size();
    std::cout<<n<<" primes computed\n";
    double outerArea=area(primes[n-1]+primes[n-2]);
    std::cout<<outerArea<<" is the outer area\n";
    double primesArea=0.0;
    for (auto &n:primes) {
        primesArea+=area(n);
    }
    std::cout<<primesArea;

}



int main() {
    int n=7;
    auto pack=Pack(primes[n-1]+primes[n-2]+1.2075,primes[n-1]);
    progressivePack(pack,n);



    //to find upper bound:
    //get n primes
    //compute area of circle r(n,n-1)
    //compute summed area of all primes
    //summed area of all primes must be less than area of outer circle







    return 0;
}