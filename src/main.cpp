#define IMG_CPP

#include "img-cpp.hpp"

int main(){

    Canvas test(100, 100);
    Canvas copytest(100, 100);
    
    //test.copy(copytest);
    
    test.saveppm("savetest.ppm");
    test.~Canvas();

    copytest.saveppm("copytest.ppm");

    return 0;
}
