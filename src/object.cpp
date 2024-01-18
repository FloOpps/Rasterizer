#include "object.h"

Object::Object(const Shape &shape, const Vec3r &translation, const Vec3r &rotation, const Vec3r &scale){
    this->shape = new Shape(shape);
    this->translation=translation;
    this->rotation=rotation;
    this->scale=scale;
}

Shape* Object::getShape(){
    return shape;
}

Object::~Object(){
    //cout << this->shape->get_name()<<endl;
    delete this->shape;
}

Object::Object(Object &&object) : shape(nullptr){
    swap(this->shape,object.shape);
}

Object::Object(const Object &object){
    this->shape = new Shape(*object.shape);
    this->translation=object.translation;
    this->rotation=object.rotation;
    this->scale=object.scale;
}

Matrix<real,4,4> Object::transform(){
    //scale, rotation, translation
    //scale
    aline::Matrix<real,4,4> scaleMatrix;
    scaleMatrix[0][0] = this->scale[0];
    scaleMatrix[1][1] = this->scale[1];
    scaleMatrix[2][2] = this->scale[2];
    scaleMatrix[3][3] = 1;


    //rotation
    //Calculate rotation about X axis
    aline::Matrix<real,4,4> rotationMatrix_x;
    rotationMatrix_x[0][0]=1;
    rotationMatrix_x[1][1]=cos(this->rotation[0]);
    rotationMatrix_x[1][2]=-sin(this->rotation[0]);
    rotationMatrix_x[2][1]=sin(this->rotation[0]);
    rotationMatrix_x[2][2]=cos(this->rotation[0]);
    rotationMatrix_x[3][3]=1;
    //Calculate rotation about Y axis
    aline::Matrix<real,4,4> rotationMatrix_y;
    rotationMatrix_y[0][0]=cos(this->rotation[1]);
    rotationMatrix_y[0][2]=sin(this->rotation[1]);
    rotationMatrix_y[1][1]=1;
    rotationMatrix_y[2][0]=-sin(this->rotation[1]);
    rotationMatrix_y[2][2]=cos(this->rotation[1]);
    rotationMatrix_y[3][3]=1;
    //Calculate rotation about Z axis
    aline::Matrix<real,4,4> rotationMatrix_z;
    rotationMatrix_z[0][0]=cos(this->rotation[2]);
    rotationMatrix_z[0][1]=-sin(this->rotation[2]);
    rotationMatrix_z[1][0]=sin(this->rotation[2]);
    rotationMatrix_z[1][1]=cos(this->rotation[2]);
    rotationMatrix_z[2][2]=1;
    rotationMatrix_z[3][3]=1;
    //multiply the rotation matrix
    aline::Matrix<real,4,4> rotationMatrix = rotationMatrix_x*rotationMatrix_y*rotationMatrix_z;

    //translate
    aline::Matrix<real,4,4> translateMatrix;
    translateMatrix[0][0] = 1;
    translateMatrix[0][3] = this->translation[0];
    translateMatrix[1][1] = 1;
    translateMatrix[1][3] = this->translation[1];
    translateMatrix[2][2] = 1;
    translateMatrix[2][3] = this->translation[2];
    translateMatrix[3][3] = 1;

    return translateMatrix * rotationMatrix * scaleMatrix;

}

void Object::makeLeftRotation(real r){
    //Y axis
    this->rotation[1]+=r;
}

void Object::makeUpRotation(real r){
    //X axis
    this->rotation[0]+=r;
}

