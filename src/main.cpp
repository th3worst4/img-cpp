#define IMG_CPP
#define BASIC_HEX

#include "img-cpp.hpp"

int main(){

    Canvas test(100, 100);

    line l1(0, 0, 99, 99, 1, RED);
    line l2(0, 99, 99, 0, 1, BLUE);
    line l3(50, 0, 51, 99, 1, GREEN);

    /*
    circ c(-25, -10, 20);
    c.fill(true, YELLOW);
    c.stroke(false);
    */

    test.line(l1);
    test.line(l2);
    test.line(l3);

    //test.circunference(c)
    test.saveppm("linetest.ppm");

    return 0;
}