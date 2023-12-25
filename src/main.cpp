#define IMG_CPP

#include "img-cpp.hpp"

int main(){

    Canvas test(100, 100);

    circ c(50, 50, 30);
    c.fill(true, BLUE);
    c.stroke(true, 1, RED);

    test.circunference(c);

    test.saveppm("savetest.ppm");

    return 0;
}
