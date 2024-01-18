#ifndef ALINE_SHAPE_H
#define ALINE_SHAPE_H

#include <vector>
#include "vertex.h"
#include "face.h"

//A data structure containing a name, list of vertices and a list of triangular faces.
struct Shape{
    private:
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<Face> faces;

    public:

        //Constructs a shape whose name, vertices and faces are given as arguments.
        Shape(const std::string &name, const std::vector<Vertex> &vertices, const std::vector<Face> &faces){
            this->name=name;
            this->vertices=vertices;
            this->faces=faces;
        }

        //Copy constructor
        Shape(const Shape &s){
            this->name = s.name;
            this->vertices = s.vertices;
            this->faces = s.faces;
        }

        //Returns the name of the face.
        std::string get_name() const{
            return name;
        }

        //Returns the list of vertices.
        std::vector<Vertex> get_vertices() const{
            return vertices;
        }

        //Returns the list of faces.
        std::vector<Face> get_faces() const{
            return faces;
        }
};

#endif