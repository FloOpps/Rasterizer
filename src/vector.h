#ifndef ALINE_VECTOR_H
#define ALINE_VECTOR_H

#include<vector>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <cmath>

using namespace std;



namespace aline{

// Define uint as an alias for unsigned int
using uint = unsigned int;

// Define real as an alias for double
using real = double;

//T type and N elements
template<class T,unsigned N>
class Vector{
    public:
        std::vector<T> vect; //better to use array

        //Default constructor
        Vector() : vect(N,T(0)){}

        //constructor with NAN
        Vector(double p) : vect(N,T(p)){}

        //pass an initialization list of elements in arguments of the constructor
        Vector(std::initializer_list<T> param){
            if(param.size()>N){ //more than N arguments, exception
                throw std::runtime_error("Size exceeds N");
            }
            else{ //less or equal than N arguments, default values
                for(typename std::initializer_list<T>::const_iterator it=param.begin(); it!=param.end(); it++){
                    vect.push_back(*it);
                }
                for(unsigned int i=param.size(); i<N; i++){
                    vect.push_back((T)0);
                }
            }
        }

        Vector(const Vector<T,N> &vCopy){
            for(typename std::vector<T>::const_iterator it=vCopy.vect.begin(); it!=vCopy.vect.end();it++){
                vect.push_back(*it);
            }
        }

        //Get value at the index gived in arguments
        T at(size_t index) const{
            if(index >= vect.size()){
                throw runtime_error("Index out of range");
            }
            return vect.at(index);
        }

        //define [] for Vector, giving const
        T operator[](size_t index) const{
            return vect[index];
        }

        //define [] for Vector with the possibility to modify
        T &operator[](size_t index){
            return vect[index];
        }

        Vector<T,N> &operator+=(const Vector &v){
            if(vect.size()!=v.vect.size()){ //peut etre pas obligé de le mettre vu que v1+=v2 compile pas si N pas le meme
                throw std::runtime_error("The Vectors need to have the same size");
            }
            for(long unsigned int i=0; i<vect.size();i++){
                vect[i]+=v.vect[i];
            }
            return *this;
        }

        //vector is norm (size of the vector)
        T norm() const{
            T sum = T(0);
            for(const T &elem : vect){
                sum+=(elem*elem);
            }
            return std::sqrt(sum);
        }

        //square of the norm (magnitude)
        T sq_norm() const{
            return norm()*norm(); //c la norm avant racine carré donc faire en sorte de l'avoir avant
        }

        //adding this for vertex (explicit copy)
        Vector<T,N> operator=(const Vector<T,N> &v2){
            for(long unsigned int i=0;i<N;i++){
                vect[i] = v2.vect[i];
            }
            return *this;
        }
};

    //vector is norm (size of the vector)
    template<class T, unsigned N>
    T norm(const Vector<T,N> &v){
        return v.norm();
    }

    //cross product : vector indicates magnitude and direction
    template<class T, unsigned N>
    Vector<T,N> cross(const Vector<T,N> &vector1, const Vector<T,N> &vector2 ){
        if(vector1.vect.size()<3 || vector2.vect.size()<3){
            throw std::runtime_error("The Vector must have at least 3 elements");
        }
        Vector<T,N> tmp = Vector<T,N>();
        tmp[0] = (vector1[1]*vector2[2]) - (vector1[2]*vector2[1]);
        tmp[1] = (vector1[2]*vector2[0]) - (vector1[0]*vector2[2]);
        tmp[2] = (vector1[0]*vector2[1]) - (vector1[1]*vector2[0]);
        return tmp;
     }

    //dot product : scalar indicates magnitude but not direction
    template<class T, unsigned N>
    T dot(const Vector<T,N> &vector1, const Vector<T,N> &vector2 ){
        size_t vect1Size = vector1.vect.size();
        if(vect1Size!=vector2.vect.size()){
            throw std::runtime_error("The Vectors need to have the same size");
        }
        T dot = (T)0;
        for(long unsigned int i=0; i<vect1Size;i++){
            dot+=(vector1[i]*vector2[i]);
        }
        return dot;
     }

     template<class T, unsigned N>
     bool isnan(const Vector<T,N> &v){
        for(const T &elem:v.vect){
            if(std::isnan(elem)) return true; //maybe to rework?
        }
        return false;
     }

    //checking if the norm of 2 T is nearly equal using a tolerence threshold
    template<class T>
    bool nearly_equal(T n1, T n2){
        bool tmp;
        const float epsilon = std::numeric_limits<float>::epsilon();
        if(n1==n2) return true;

        tmp = n1>n2?((n1-n2)<abs(n1)*epsilon):((n2-n1)<abs(n2)*epsilon);
        return tmp;
        
    }

    //checking if the norm of 2 vectors is nearly equal using a tolerence threshold
    //difference de A et B < pourcentage de A (A*p) p =pourcentage %
    template<class T, unsigned N>
    bool nearly_equal(const Vector<T,N> &v1, const Vector<T,N> &v2){
        for(unsigned int i=0;i<N;i++){
            if(!nearly_equal(v1.vect[i],v2.vect[i])) return false;
        }
        return true;
    }

    //checking if a vector is unitary
     template<class T, unsigned N>
     bool is_unit(const Vector<T,N> &v){
        return nearly_equal((T)v.norm(),(T)1);
     }

   

    template<class T, unsigned N>
    bool operator==(const Vector<T,N> &v1, const Vector<T,N> &v2){
        size_t size= v1.vect.size();
        if(size!=v2.vect.size()) return false;
        for(long unsigned int i=0;i<size;i++){
            if(v1.vect[i]!=v2.vect[i]) return false;
        }
        return true;
    }

    template<class T, unsigned N>
    bool operator!=(const Vector<T,N> &v1, const Vector<T,N> &v2){
        if(v1==v2) return false;
        return true;
    }

    template<class T, unsigned N>
    std::ostream &operator<<(std::ostream &out, const Vector<T,N> &v){
        out << "(";
        for(unsigned int i=0;i<N;i++){
            if(i==N-1) out << v[i];
            else out << v[i] << " ; ";
        }
        out << ")";
        return out;
    }

    template<class T,unsigned N>
    Vector<T,N> operator+(const Vector<T,N> &v, const Vector<T,N> &v2){
        Vector<T,N> tmp;
        for(unsigned int i=0;i<N;i++){
            tmp.vect[i]=v[i]+v2[i];
        }
        return tmp;
    }

    template<class T,unsigned N>
    Vector<T,N> operator-(const Vector<T,N> &v){
        Vector<T,N> tmp(v);
        for(T &elem: tmp.vect){
            elem = -(elem);
        }
        return tmp;
    }

    template<class T,unsigned N>
    Vector<T,N> operator-(const Vector<T,N> &v, const Vector<T,N> &v2){
        Vector<T,N> tmp;
        for(unsigned int i=0; i<N;i++){
            tmp[i] = v[i] - v2[i];
        }
        return tmp;
    }

    //product of a scalar and a vector
    template<class T,unsigned N>
    Vector<T,N> operator*(const T &scalar, const Vector<T,N> &v){
        Vector<T,N> tmp(v);
        for(T &elem : tmp.vect){
            elem = elem*scalar;
        }
        return tmp;
    }

    //product of a scalar and a vector
    template<class T,unsigned N>
    Vector<T,N> operator*(const Vector<T,N> &v, const T &scalar){
        return (T)scalar*v;
    }

    //division par 0 impossible
    template<class T,unsigned N>
    Vector<T,N> operator/(const Vector<T,N> &v, const T &scalaire){
        if(scalaire==0) throw std::runtime_error("Scalaire have to be more than 0");
        Vector<T,N> tmp(v);
        for(T &elem : tmp.vect){
            elem = elem/scalaire;
        }
        return tmp;
    }

    //to string a vector
    template<class T, unsigned N>
    std::string to_string(const Vector<T,N> &v){
        std::string tmp = "(";
        for(const T &elem: v.vect){
            tmp += std::to_string(elem) + ", ";
        }
        return tmp.erase(tmp.size() -2, 2)+")";
    }

    //normalize a vector
    template<class T,unsigned N>
    Vector<T,N> unit_vector(const Vector<T,N> &v){
        T size = v.norm();
        Vector<T,N> tmp(v);
        for(T &elem : tmp.vect){
            elem = elem/size;
        }
        return tmp;
    }

    // Define Vec2i as an alias for Vector<int, 2>
    using Vec2i = Vector<int, 2>;

    // Define Vec2r as an alias for Vector<real, 2>
    using Vec2r = Vector<real, 2>;

    // Define Vec3r as an alias for Vector<real, 3>
    using Vec3r = Vector<real, 3>;

    // Define Vec4r as an alias for Vector<real, 4>
    using Vec4r = Vector<real, 4>;
}

#endif