#define IMG_CPP
#define BASIC_HEX

#include "img-cpp.hpp"

int main(){

    Canvas test(100, 100);

    line l1(10, 10, 77, 77, 1, RED);
    line l2(10, 77, 88, 44, 1, BLUE);
    line l3(0, 50, 99, 60, 1, GREEN);
    line l4(0, 50, 99, 51, 1, BLACK);
    line l5(50, 0, 50, 99, 1, WHITE);

    /*
    circ c(-25, -10, 20);
    c.fill(true, YELLOW);
    c.stroke(false);
    */

    test.line(l1);
    test.line(l2);
    test.line(l3);
    test.line(l4);
    test.line(l5);

    //test.circunference(c)
    test.saveppm("linetest.ppm");

    return 0;
}