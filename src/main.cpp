#define IMG_CPP

#include "img-cpp.hpp"

int main(){

    Canvas test(100, 100);
    Canvas test2(50, 50);

    test.change_pixel(50, 50, RED);

    test.saveppm("savetest.ppm");
    test2.saveppm("copytest.ppm");

    return 0;
}
