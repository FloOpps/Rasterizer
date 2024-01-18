#ifndef ALINE_OBJECT_H
#define ALINE_OBJECT_H
#include "shape.h"
#include "matrix.h"
#include <math.h>


class Object{
    private:
        Shape *shape;
        Vec3r translation,rotation,scale;

    public:
        Object(const Shape &shape, const Vec3r &translation, const Vec3r &rotation, const Vec3r &scale);
        Matrix<real,4,4> transform();
        Shape *getShape();
        ~Object();
        Object(Object &&object);
        Object(const Object &object);
        void makeLeftRotation(real r);
        void makeUpRotation(real r);

};
#endif