#include "../include/color.h"

using namespace minwin;

//A data structure containing three indexes and a color.
struct Face{
    unsigned int v0,v1,v2;
    Color color;

    //Constructs a triangular face whose vertex indexes and color are the ones given as arguments.
    Face(uint v0, uint v1, uint v2, const Color &color){
        this->v0=v0;
        this->v1=v1;
        this->v2=v2;
        this->color=color;
    }

    //with default color
    Face(uint v0, uint v1, uint v2){
        this->v0=v0;
        this->v1=v1;
        this->v2=v2;
        minwin::Color  colorBlue  { minwin::BLUE };
        this->color=colorBlue;
    }
};