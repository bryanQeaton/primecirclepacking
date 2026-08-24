#ifndef PRIMECIRCLEPACKING_RANDOMPACK_H
#define PRIMECIRCLEPACKING_RANDOMPACK_H
#include <random>

#include "circles.h"
#include "view.h"

inline std::mt19937 gen(std::random_device{}());

void randomPack() {
    auto outer=Circle(0.0,0.0,12.1);
    std::uniform_real_distribution<double> dist(-(outer.r-2.0),outer.r-2.0);
    auto c=Circle(dist(gen),dist(gen),2.0);
    auto c1=Circle(dist(gen),dist(gen),3.0);
    auto c2=Circle(dist(gen),dist(gen),5.0);
    auto c3=Circle(dist(gen),dist(gen),7.0);
    while (!isInside(c,outer)||
        !isInside(c1,outer)||
        !isInside(c2,outer)||
        !isInside(c3,outer)||
        isIntersecting(c,c1)||
        isIntersecting(c,c2)||
        isIntersecting(c,c3)||
        isIntersecting(c1,c2)||
        isIntersecting(c1,c3)||
        isIntersecting(c2,c3)
        ) {
        c=Circle(dist(gen),dist(gen),2.0);
        c1=Circle(dist(gen),dist(gen),3.0);
        c2=Circle(dist(gen),dist(gen),5.0);
        c3=Circle(dist(gen),dist(gen),7.0);
    }
    std::vector<Circle> circs={outer,c,c1,c2,c3};
    rasterizeCirclesBMP(circs,"circles.bmp",1024,1.0);

}



#endif //PRIMECIRCLEPACKING_RANDOMPACK_H
