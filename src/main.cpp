#define IMG_CPP

#include "img-cpp.hpp"

int main(){

    Canvas test(100, 100);
    test.save("savetest.ppm");

    return 0;
}
