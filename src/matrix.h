#ifndef ALINE_MATRIX_H
#define ALINE_MATRIX_H

#include "vector.h"
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <cmath>

using namespace std;

namespace aline{
//M lines per N columns
//A Vector represents one row in the matrix, all is elements is the number of columns
template<class T, unsigned M, unsigned N>
class Matrix{
    public:
        std::vector<Vector<T,N>> matrix;
    
        //default constructor for a matrix filled up with zeros, calling the Vector default constuctor
        Matrix() : matrix(M, Vector<T,N>()){}

        //constructor filled of the paramater
        Matrix(T p) : matrix(M, Vector<T,N>(p)){}

        //Constructs a matrix with the vectors given as arguments. Each vector is one line of
        //the matrix
        Matrix(std::initializer_list<Vector<T,N>> listVector){
            for(const Vector<T,N> &elem : listVector){
                matrix.push_back(elem);
            }
            if(listVector.size()<M){
                unsigned int reste = M-listVector.size();
                for(unsigned int i = 0; i<reste; i++){
                    matrix.push_back(Vector<T,N>());
                }
            }
        }

        //copy constructor
        Matrix(const Matrix<T,M,N> &m){
            for(const Vector<T,N> &elem : m.matrix){
                matrix.push_back(elem);
            }
        }

        //get row
        const Vector<T,N> &at(size_t index) const{
            if(index>=M) throw std::runtime_error("Index of the row is out of range");
            return matrix[index];
        }

        //get row and possibility to modify the Vector
        Vector<T,N> &at(size_t index){
            if(index>=M) throw std::runtime_error("Index of the row is out of range");
            return matrix[index];
        }

        //get element at index at the line
        T at(size_t line, size_t index){
            Vector<T,N> tmp = at(line);
            if(index>=N) throw std::runtime_error("Index of the element is out of range");
            return tmp.vect[index];
        }
        

        const Vector<T,N> &operator[](size_t index) const{
            return at(index);
        }

        Vector<T,N> &operator[](size_t index){
            return at(index);
        }

        Matrix<T,M,N> &operator+=(const Matrix<T,M,N> &m){
            for(unsigned int  row=0;row<M;row++){
                for(unsigned int  column=0; column<N; column++){
                    matrix[row][column]+=m.matrix[row][column];
                }
            }
                return *this;
        }

};

//cheacking if the matrix is equal to its identity
template<class T, unsigned M>
bool identity(const Matrix<T,M,M> &m){
    for(unsigned int  row=0;row<M;row++){
        for(unsigned int  column=0;column<M;column++){
            if(row==column && m.matrix[row][column]!=1) return false;
            else if(row!=column && m.matrix[row][column]!=0) return false;
        }
    }
    return true;
}

//Compute derterminant of the matrix, using recursion for an nxn matrix
template<class T,unsigned M>
T determinant(const Matrix<T,M,M> &m){
    T result=0;
    //must use constexpr, or compilation will fail, we can calcule this during compilation
    if constexpr(M==1){ //matrix 1x1
        return m.matrix[0][0];
    }
    else if constexpr(M==2){ //matrix 2x2
        return ((m.matrix[0][0]*m.matrix[1][1]) - (m.matrix[0][1]*m.matrix[1][0]));
    }
    else{ //matrix nxn
        for(unsigned column=0;column<M;column++){
            result+= m.matrix[0][column]*pow(-1,column)*matrixPartition(m,0,column);
        }
    }
    return result;
}

//remove column/line from the adjacent method to call determinant
template<class T,unsigned M,unsigned N>
T matrixPartition(const Matrix<T,M,N> &m, int rowToDelete, int columnToDelete){
    Matrix<T,M-1,N-1> tmpMatrix;
    unsigned rowTmp=0; //rowTmp is the variable for the tmp matrix
    unsigned columnTmp=0; //column is the variable for the tmp matrix
    for(unsigned int row=0;row<M;row++){
        if(row!=rowToDelete){
            for(unsigned int column=0;column<N;column++){
                if(column!=columnToDelete){
                    tmpMatrix.matrix[rowTmp][columnTmp] = m.matrix[row][column];
                    columnTmp++;
                }
            }
        columnTmp=0;
        rowTmp++;
        }
    }
    return determinant(tmpMatrix);
}

//Compute the adjugate/cofactor of a matrix.
template<class T, unsigned M, unsigned N>
Matrix<T,M,N> adjacent(const Matrix<T,M,N> &m){ //ou cofacteur?
    Matrix<T,M,N> tmp;
    for(unsigned int row=0;row<M;row++){
        for(unsigned int column=0;column<N;column++){
            tmp[row][column] = pow(-1,row+column)*matrixPartition(m,row,column);
        }
    }
    return tmp;
}

//Compute the inverse of a matrix, using adjacent, transopose, determinant
//inverse = (1/determinant(m))*transpose(adjacent(m))
template<class T, unsigned M>
Matrix<T,M,M> inverse(const Matrix<T,M,M> &m){
    Matrix<T,M,M> tmp;
    tmp = adjacent(m);
    T det = determinant(m);
    if(det==0){ //NAN matrix
        return Matrix<T,M,M>(NAN);
    }
    Matrix<T,M,M> inv = (1/det) * transpose(tmp);
    if(!(identity(inv*m))) throw std::runtime_error("There is a problem with the inverse matrix");
    return inv;
}

//test if matrix contain NaN number
template<class T, unsigned M, unsigned N>
bool isnan(const Matrix<T,M,N> &m){
    for(const Vector<T,N> &elem : m.matrix){
        if(isnan(elem)) return true;
    }
    return false;
}

//test if two matrix are equals
template<class T, unsigned M, unsigned N>
bool operator==(const Matrix<T,M,N> &m,const Matrix<T,M,N> &m2){
    for(unsigned int row=0; row<M; row++){
        for(unsigned int column=0;column<N;column++){
            if(m.matrix[row][column]!=m2.matrix[row][column]) return false;
        }
    }
    return true;
}

//test if two matrix are not equals
template<class T, unsigned M, unsigned N>
bool operator!=(const Matrix<T,M,N> &m,const Matrix<T,M,N> &m2){
    return !(m==m2);
}

//output operator
template<class T, unsigned M, unsigned N>
ostream &operator<<(ostream &out, const Matrix<T,M,N> &m){
    out << "[";
    for(unsigned int i=0; i<M; i++){
        out << m.matrix[i];
        if(i!=M-1) out << endl;
    }
    return out << "]";
}

//sum of two matrix
template<class T, unsigned M, unsigned N>
Matrix<T,M,N> operator+(const Matrix<T,M,N> &m, const Matrix<T,M,N> &m2){
    Matrix<T,M,N> tmp;
    for(unsigned int row=0;row<M;row++){
        for(unsigned int column=0;column<N;column++){
            tmp.matrix[row][column] = m.matrix[row][column] + m2.matrix[row][column];
        }
    }
    return tmp;
}

//the negation of a matrix
template<class T, unsigned M, unsigned N>
Matrix<T,M,N> operator-(const Matrix<T,M,N> &m){
    Matrix<T,M,N> tmp(m);
    for(Vector<T,N> &elem : tmp.matrix){
        elem = -elem;
    }
    return tmp;
}

//the substract of two matrix
template<class T, unsigned M, unsigned N>
Matrix<T,M,N> operator-(const Matrix<T,M,N> &m, const Matrix<T,M,N> &m2){
    Matrix<T,M,N> tmp;
    for(unsigned int row = 0; row<M; row++){
        for(unsigned int column=0;column<N;column++){
            tmp.matrix[row][column] = m.matrix[row][column] - m2.matrix[row][column];
        }
    }
    return tmp;
}

//the product of a scalar and a matrix
template<class T, unsigned M, unsigned N>
Matrix<T,M,N> operator*(const T &scalaire, const Matrix<T,M,N> &m){
    Matrix<T,M,N> tmp;
    for(unsigned int row = 0; row<M; row++){
        for(unsigned int column=0;column<N;column++){
            tmp.matrix[row][column] = scalaire * m.matrix[row][column];
        }
    }
    return tmp;
}

//the product of a matrix and a scalar
template<class T, unsigned M, unsigned N>
Matrix<T,M,N> operator*(const Matrix<T,M,N> &m, const T &scalaire){
    return scalaire*m;
}

//the product of a matrix and a vector
template<class T, unsigned M, unsigned N>
Vector<T,M> operator*(const Matrix<T,M,N> &m, const Vector<T,N> &v){
    Vector<T,M> tmpVector;
    for(unsigned int row=0;row<M;row++){
        for(unsigned int column=0;column<N;column++){
            tmpVector.vect[row] += m.matrix[row][column]*v.vect[column];
        }
    }
    return tmpVector;
}

//the product of two matrix
//nbr column 1ere == nbr ligne 2eme
template<class T, unsigned M, unsigned N, unsigned O>
Matrix<T,M,O> operator*(const Matrix<T,M,N> &m, const Matrix<T,N,O> &m2){

    Matrix<T,M,O> tmpMatrix;

    for(unsigned int row=0;row<M;row++){
        for(unsigned int column=0;column<O;column++){
            for(unsigned int columnProd=0;columnProd<N;columnProd++){
                tmpMatrix.matrix[row][column] += (m.matrix[row][columnProd]*m2.matrix[columnProd][column]);
            }
        }
    }
    return tmpMatrix;
}

template<class T, unsigned M, unsigned N>
Matrix<T,M,N> operator/(const Matrix<T,M,N> &m,const T &e){
    return m*(1/e);
}

//to string a matrix
template<class T, unsigned M, unsigned N>
    std::string to_string(const Matrix<T,M,N> &m){
        std::string tmp = "(";
        for(const Vector<T,N> &elem: m.matrix){
            tmp += to_string(elem) + ", ";
        }
        tmp.erase(tmp.size() -2, 2);
        return tmp += ")";
    }

//computing the transpose of a matrix
 template<class T, unsigned M, unsigned N>
    Matrix<T,M,N> transpose(const Matrix<T,M,N> &m){
        Matrix<T,N,M> tmp;
        for(unsigned int row=0; row<M; row++){
            for(unsigned int column=0;column<N;column++){
                tmp[column][row] = m.matrix[row][column];
            }
        }
        return tmp;
    }

    //nearly equal 2 matrix
    template<class T,unsigned M,unsigned N>
    bool nearly_equal(const Matrix<T,M,N> &m, const Matrix<T,M,N> &m2){
        for(unsigned int row=0;row<M;row++){
            for(unsigned int column=0;column<N;column++){
                if(!nearly_equal(m[row][column],m2[row][column])) return false;
            }
        }
        return true;
    }
}

#endif