#ifndef PRIMECIRCLEPACKING_CIRCLES_H
#define PRIMECIRCLEPACKING_CIRCLES_H
#include <cmath>

struct Circle {
    double x{};
    double y{};
    double r{};
    auto operator<=>(const Circle &) const =default;
};

inline bool isIntersecting(const Circle &c1,const Circle &c2) {
    const double dist=std::sqrt(pow(c1.x-c2.x,2.0)+pow(c1.y-c2.y,2.0));
    if (c1.r+c2.r>dist){return true;}
    return false;
}

inline bool isInside(const Circle &c1,const Circle &outer) {
    const double dist=std::sqrt(pow(c1.x-outer.x,2.0)+pow(c1.y-outer.y,2.0));
    if (dist+c1.r>outer.r){return false;}
    return true;
}




#endif //PRIMECIRCLEPACKING_CIRCLES_H
