#ifndef PRIMECIRCLEPACKING_PACK_H
#define PRIMECIRCLEPACKING_PACK_H
#include "circles.h"
#include "view.h"

struct Pack {
    Circle outer{};
    std::vector<Circle> inners{};
    Pack(const double outerR,const double firstR) {
        outer.r=outerR;
        inners.emplace_back(0.0,-outerR+firstR,firstR);
    }
    [[nodiscard]] std::vector<Circle> potentials(const double newRadius) const {
        std::vector<Circle> intersectors{};
        std::vector<Circle> pots{};
        intersectors.emplace_back(0,0,outer.r-newRadius);
        for (const auto &n:inners) {
            intersectors.emplace_back(n.x,n.y,n.r+newRadius);
        }
        for (int n=0;n<intersectors.size();n++) {
            for (int m=n;m<intersectors.size();m++) {
                if (m==n){continue;}
                const double dx=intersectors[m].x-intersectors[n].x;
                const double dy=intersectors[m].y-intersectors[n].y;
                const double d2=dx*dx+dy*dy;
                if (d2==0.0){continue;}
                const double d=sqrt(d2);
                const double r1=intersectors[n].r;
                const double r2=intersectors[m].r;
                if (d>r1+r2||d<fabs(r1-r2)){continue;}
                const double l=(r1*r1-r2*r2+d2)/(2.0*d);
                const double h2=r1*r1-l*l;
                const double h=sqrt(std::max(0.0,h2));
                const double px=intersectors[n].x+l*dx/d;
                const double py=intersectors[n].y+l*dy/d;
                auto p1=Circle(px+h*dy/d,py-h*dx/d,newRadius);
                auto p2=Circle(px-h*dy/d,py+h*dx/d,newRadius);
                if (isInside(p1,outer)) {
                    pots.push_back(p1);
                }
                if (p1!=p2&&isInside(p2,outer)) {
                    pots.push_back(p2);
                }


            }
        }

        return pots;
    }

    void view(const std::string& filename) const {
        auto vec=inners;
        vec.push_back(outer);
        rasterizeCirclesBMP(vec,filename.c_str(),100);

    }
};


#endif //PRIMECIRCLEPACKING_PACK_H
