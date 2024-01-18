#ifndef ALINE_VERTEX_H
#define ALINE_VERTEX_H

#include "vector.h"

using namespace aline;

//A data structure containing the coordinates of a point plus some information used by the rasterizer
struct Vertex {
    Vec2r c;
    Vec3r c3d;
    real h;

    //Constructs a vertex whose coordinates are c and the color intensity is h 2D
    Vertex(const Vec2r &c, real h) : c(c), h(h){}

    //Constructs a vertex whose coordinates are c and the color intensity is h 3D
    Vertex(const Vec3r &c, real h) : c3d(c), h(h){}
};

#endif