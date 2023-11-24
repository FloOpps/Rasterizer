#include <vector>

namespace aline{

    class Vector{
        public:
            //Get value at the index gived in arguments
        T at(size_t index) const{}

        //define [] for Vector, giving const
        T operator[](size_t index) const{}

        //define [] for Vector with the possibility to modify
        T &operator[](size_t index){}

        Vector<T,N> &operator+=(const Vector &v){}

        //vector is norm (size of the vector)
        T norm() const{}

        //square of the norm (magnitude)
        T sq_norm() const{}
    };

    template<class T, unsigned N>
    Vector<T,N> cross(const Vector<T,N> &vector1, const Vector<T,N> &vector2 ){}

    //dot product
    template<class T, unsigned N>
    T dot(const Vector<T,N> &vector1, const Vector<T,N> &vector2 ){}

     template<class T, unsigned N>
     bool isnan(const Vector<T,N> &v){}

     template<class T, unsigned N>
     bool is_unit(const Vector<T,N> &v){}

    template<class T, unsigned N>
    bool nearly_equal(const Vector<T,N> &v1, const Vector<T,N> &v2){}

    template<class T, unsigned N>
    bool operator==(const Vector<T,N> &v1, const Vector<T,N> &v2){}

    template<class T, unsigned N>
    bool operator!=(const Vector<T,N> &v1, const Vector<T,N> &v2){}

    template<class T, unsigned N>
    std::ostream &operator<<(std::ostream &out, const Vector<T,N> &v){}

    template<class T,unsigned N>
    Vector<T,N> operator+(const Vector<T,N> &v, const Vector<T,N> &v2){}

    template<class T,unsigned N>
    Vector<T,N> operator-(const Vector<T,N> &v){}

    template<class T,unsigned N>
    Vector<T,N> operator-(const Vector<T,N> &v, const Vector<T,N> &v2){}

    //product of a scalar and a vector
    template<class T,unsigned N>
    Vector<T,N> operator*(const T &scalar, const Vector<T,N> &v){}

    //product of a scalar and a vector
    template<class T,unsigned N>
    Vector<T,N> operator*(const Vector<T,N> &v, const T &scalar){}

    //division par 0 impossible
    template<class T,unsigned N>
    Vector<T,N> operator/(const Vector<T,N> &v, const T &scalaire){}

    template<class T, unsigned N>
    std::string to_string(const Vector<T,N> &v){}

    template<class T,unsigned N>
    Vector<T,N> unit_vector(const Vector<T,N> &v){}
}