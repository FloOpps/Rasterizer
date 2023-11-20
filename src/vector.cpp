#include "vector.h"

//uniquement pour l'executable, si template coder uniquement dans .h
int main(){
    std::initializer_list<float> numbers1 = {1.0,1.15};
    std::initializer_list<float> numbers2 = {1.0,1.10};

    Vector<float,2> v1(numbers1);
    Vector<float,2> v2(numbers2);
    std::cout << float(0);
    return 0;
}