#define IMG_CPP
#define BASIC_HEX

#include "img-cpp.hpp"

int main(){

    Canvas test("savetest.ppm");
    Canvas readtest = Canvas(100, 100);

    test.copy(readtest);
    readtest.saveppm("readtest.ppm");

    return 0;
}