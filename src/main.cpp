#define IMG_CPP

#include "img-cpp.hpp"

int main(){

    Canvas test(100, 100);

    line l(50, 50, 50, 80, 1, 0);

    test.line(l);

    test.saveppm("savetest.ppm");

    return 0;
}
