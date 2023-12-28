#define IMG_CPP
#define BASIC_HEX

#include "img-cpp.hpp"

int main(){

    Canvas test(100, 100);

    line l1(0, 0, 99, 99, 0, RED);
    line l2(0, 99, 99, 0, 0, RED);

    test.line(l1);
    test.line(l2);

    test.saveppm("linetest.ppm");

    return 0;
}