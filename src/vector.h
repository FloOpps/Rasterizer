#include<vector>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <cmath>

using namespace std;


//T type and N elements
template<class T,unsigned N>
class Vector{
    public:
        std::vector<T> vect;

        //Default constructor
        Vector() : vect(N,T(0)){}

        //constructor with NAN
        Vector(double p) : vect(N,T(p)){}

        //pass an initialization list of elements in arguments of the constructor
        Vector(std::initializer_list<T> param){
            if(param.size()>N){ //more than N arguments, exception
                throw std::runtime_error("Size exceeds N");
            }
            else if(param.size()<N){ //less than N arguments, default values
                vect = std::vector<T>(N,T(0));
            }
            else{
                for(typename std::initializer_list<T>::const_iterator it=param.begin(); it!=param.end(); it++){
                    vect.push_back(*it);
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
            if(index > vect.size()-1){
                throw std::runtime_error("Index out of range");
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
            for(int i=0; i<vect.size();i++){
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
            return norm()*norm();
        }
};

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
        for(int i=0; i<vect1Size;i++){
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

    //checking if a vector is unitary
     template<class T, unsigned N>
     bool is_unit(const Vector<T,N> &v){
        T sum = T(0);
        for(const T &elem : v.vect){
            sum+=(elem*elem);
        }
        if(std::sqrt(sum)!=T(1)) return false;
        else return true;
     }

    //checking if the norm of 2 vectors is nearly equal using a tolerence threshold
    template<class T, unsigned N>
    bool nearly_equal(const Vector<T,N> &v1, const Vector<T,N> &v2){
        T normV1 = v1.norm();
        T normV2 = v2.norm();
        T percentage = normV1>normV2?((normV1-normV2)/normV1):((normV2-normV1)/normV2);
        return percentage<0.05;
    }

    template<class T, unsigned N>
    bool operator==(const Vector<T,N> &v1, const Vector<T,N> &v2){
        size_t size= v1.vect.size();
        if(size!=v2.vect.size()) return false;
        for(int i=0;i<size;i++){
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
        for(int i=0;i<N;i++){
            if(i==N-1) out << v[i];
            else out << v[i] << " ; ";
        }
        out << ")";
        return out;
    }

    template<class T,unsigned N>
    Vector<T,N> operator+(const Vector<T,N> &v, const Vector<T,N> &v2){
        Vector<T,N> tmp;
        for(int i=0;i<N;i++){
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
        for(int i=0; i<N;i++){
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
        std::string tmp = "";
        for(const T &elem: v.vect){
            tmp += std::to_string(elem) + " ";
        }
        return tmp.erase(tmp.size() -1, 1);
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
