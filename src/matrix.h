#include "vector.h"
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <cmath>

using namespace std;

//M lines per N columns
//A Vector represents one row in the matrix, all is elements is the number of columns
template<class T, unsigned M, unsigned N>
class Matrix{
    public:
        std::vector<Vector<T,N>> matrix;
    
    public:

        //default constructor
        Matrix() : matrix(M, Vector<T,N>()){}

        //Constructs a matrix with the vectors given as arguments. Each vector is one line of
        //the matrix
        Matrix(std::initializer_list<Vector<T,N>> listVector){
            for(const Vector<T,N> &elem : listVector){
                matrix.push_back(elem);
            }
        }

        //copy constructor
        Matrix(const Matrix<T,M,N> &m){
            for(const Vector<T,N> &elem : m.matrix){
                matrix.push_back(elem);
            }
        }

        //get row
        Vector<T,N> at(size_t index) const{
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
            for(int row=0;row<M;row++){
                for(int column=0; column<N; column++){
                    matrix[row][column]+=m.matrix[row][column];
                }
            }
                return *this;
        }
};

template<class T, unsigned M, unsigned N>
bool identity(const Matrix<T,M,N> &m){
    for(int row=0;row<M;row++){
        for(int column=0;column<N;column++){
            if(row==column && m.matrix[row][column]!=1) return false;
            else if(row!=column && m.matrix[row][column]!=0) return false;
        }
    }
    return true;
}


/*
template<class T, unsigned M, unsigned N>
Matrix<T,M,N> inverse(const Matrix<T,M,N> &m){
    Matrix<T,M,N> tmp;
    if(N!=M) throw std::runtime_error("This matrix can't be inversed");
    

}
*/


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
    for(int row=0; row<M; row++){
        for(int column=0;column<N;column++){
            if(m.matrix[row][column]!=m2.matrix[row][column]) return false;
        }
    }
    return true;
}

//test if two matrix are equals
template<class T, unsigned M, unsigned N>
bool operator!=(const Matrix<T,M,N> &m,const Matrix<T,M,N> &m2){
    return !(m==m2);
}

//output operator
template<class T, unsigned M, unsigned N>
ostream &operator<<(ostream &out, const Matrix<T,M,N> &m){
    out << "[";
    for(int i=0; i<M; i++){
        out << m.matrix[i];
        if(i!=M-1) out << endl;
    }
    return out << "]";
}

//sum of two matrix
template<class T, unsigned M, unsigned N>
Matrix<T,M,N> operator+(const Matrix<T,M,N> &m, const Matrix<T,M,N> &m2){
    Matrix<T,M,N> tmp;
    for(int row=0;row<M;row++){
        for(int column=0;column<N;column++){
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
    for(int row = 0; row<M; row++){
        for(int column=0;column<N;column++){
            tmp.matrix[row][column] = m.matrix[row][column] - m2.matrix[row][column];
        }
    }
    return tmp;
}

//the product of a scalar and a matrix
template<class T, unsigned M, unsigned N>
Matrix<T,M,N> operator*(const T &scalaire, const Matrix<T,M,N> &m){
    Matrix<T,M,N> tmp;
    for(int row = 0; row<M; row++){
        for(int column=0;column<N;column++){
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
Vector<T,N> operator*(const Matrix<T,M,N> &m, const Vector<T,N> &v){
    Vector<T,N> tmpVector;
    for(int row=0;row<M;row++){
        for(int column=0;column<N;column++){
            tmpVector.vect[row] += m.matrix[row][column]*v.vect[column];
        }
    }
    return tmpVector;
}

//the product of two matrix
template<class T, unsigned M, unsigned N>
Matrix<T,M,N> operator*(const Matrix<T,M,N> &m, const Matrix<T,M,N> &m2){
    Matrix<T,M,N> tmpMatrix;
    for(int row=0;row<M;row++){
        for(int column=0;column<N;column++){
            for(int columnProd=0;columnProd<N;columnProd++){
                tmpMatrix.matrix[row][column] += (m.matrix[row][columnProd]*m2.matrix[columnProd][column]);
            }
        }
    }
    return tmpMatrix;
}


template<class T, unsigned M, unsigned N>
    std::string to_string(const Matrix<T,M,N> &m){
        std::string tmp = "[";
        for(const Vector<T,N> &elem: m.matrix){
            tmp += to_string(elem) + "\n";
        }
        tmp.erase(tmp.size() -1, 1);
        return tmp += "]";
    }

 template<class T, unsigned M, unsigned N>
    Matrix<T,M,N> transpose(const Matrix<T,M,N> &m){
        Matrix<T,N,M> tmp;
        for(int row=0; row<M; row++){
            for(int column=0;column<N;column++){
                tmp[column][row] = m.matrix[row][column];
            }
        }
        return tmp;
    }