#include <glm/glm.hpp>
#include "Noise.h"

namespace{std::array<glm::vec2,7>vectors{
    glm::vec2(-1.0, 1.0),
    glm::vec2(-1.0, -1.0),
    glm::vec2(1.0, 1.0),
    glm::vec2(1.0, -1.0),
};
}




double Noise::noise2D(double x, double y){
    x = glm::abs(x);
    y = glm::abs(y);
    int xf = (double)(static_cast<int>(x));
    int yf = (double)(static_cast<int>(y));
    double u = x-xf;
    double v = y-yf;
    xf = xf%255;
    yf = yf%255;

    glm::vec2 bLeft{u,v};
    glm::vec2 tLeft{u,-(1-v)};
    glm::vec2 bRight{-(1-u),v};
    glm::vec2 tRight{-(1-u),-(1-v)};

    int valueTopRight = m_perms[m_perms[xf+1]+yf+1];
    int valueTopLeft = m_perms[m_perms[xf]+yf+1];
    int valueBottomRight = m_perms[m_perms[xf+1]+yf];
    int valueBottomLeft = m_perms[m_perms[xf]+yf];

    double dotTopRight = glm::dot(tRight,vectors[valueTopRight%4]);
    double dotTopLeft = glm::dot(tLeft,vectors[valueTopLeft%4]) ;
    double dotBottomRight = glm::dot(bRight,vectors[valueBottomRight%4]) ;
    double dotBottomLeft = glm::dot(bLeft,vectors[valueBottomLeft%4]) ;
    
    u = fade(u);
    v = fade(v);

    return lerp(u,lerp(v,dotBottomLeft,dotTopLeft),lerp(v,dotBottomRight,dotTopRight));
}


double Noise::fade(double u){
    return((((6*u-15)*u)+10)*u*u*u);
}

double Noise::lerp(double a, double b, double c){
    return b+a*(c-b);
}

double Noise::fbm(double x, double y){
    double freq = frequency;
    double amp = amplitude;
    double n{0};
    for(int i{0};i<octaves;i++){
        n+=amp*noise2D(x*freq,y*freq);
        freq*=2;
        amp*=0.5;
    }
    return n>1?1.0:n;
}
