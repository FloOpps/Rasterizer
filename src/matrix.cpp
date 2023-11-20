#include "matrix.h"
using namespace std;

int main(){

    std::initializer_list<float> numbers1 = {1.0,1.10};
    std::initializer_list<float> numbers2 = {1.0,1.15};

    Vector<float,2> v1(numbers1);
    Vector<float,2> v2(numbers2);

    std::initializer_list<Vector<float,2>> vectVect = {v1,v2};
    std::initializer_list<Vector<float,2>> vectVect2 = {v2,v1};

    Matrix<float,2,2> m(vectVect);
    Matrix<float,2,2> m2(vectVect2);

    Matrix<float,2,2> v = m*m2;
    std::cout << to_string(m);
    cout << endl;
    cout << transpose(m);


    return 0;
}