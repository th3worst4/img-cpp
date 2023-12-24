#define IMG_CPP

#include "img-cpp.hpp"

int main(){

    Canvas test(100, 100);
    Canvas copytest(100, 100);
    

    test.change_pixel(50, 50, RED);
    test.copy(copytest);
    
    copytest.saveppm("copytest.ppm");

    return 0;
}
