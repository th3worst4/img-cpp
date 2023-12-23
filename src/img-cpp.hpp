/*
    The main goal of this project is to create a header only library that
    allows simple image creation/modification.

    The ideia of this whole thing was taken from the Tsoding channel;
    I thought it would be a nice challenge to me to reacreate his ideias
    but by my own
*/


#include <iostream>
#include <fstream>
#include <stdint.h>
#include <array>
#include <string>
#include <algorithm>
#include <iterator>


#ifdef IMG_CPP

enum ERR{
    NO_ERR = 0,
    OPEN_ERR = 1,
    COPY_ERR = 2,
};

struct Canvas{
    size_t w, h;
    size_t size;
    uint8_t *data = NULL;

    Canvas(size_t w, size_t h);
    ~Canvas();

    int saveppm(const char* name);
    int copy(Canvas &out);
};

Canvas::Canvas(size_t w, size_t h): w(w), h(h){
    size = w*h*3;
    data = new uint8_t[size];
    std::fill(data, data+size, 0);
}

Canvas::~Canvas(){
    delete data;
}

int Canvas::saveppm(const char* name){
    std::fstream canvas(name, std::ios::out);

    if(!canvas.is_open()) return OPEN_ERR;

    canvas << "P3\n" << w << ' ' << h << "\n255\n";

    for(int i; i < size; i++){
        canvas << (int)*(data+i) << ' ';
        if((i+1)%3 == 0){
            canvas << "\n";
        }
    }

    canvas.close();
    return NO_ERR;
}

int Canvas::copy(Canvas &out){
    if(w != out.w || h != out.h){
        std::cout << "(img-cpp) Canvas don't have same size. Rescale first\n";
        return COPY_ERR;
    }

    for(int i = 0; i < size; i++) *(out.data+i) = *(data+i);

    return NO_ERR;
}

enum HEX_COLOR{
    WHITE = 0xFFFFFF,
    BLACK = 0x000000,

    RED = 0xFF0000,
    GREEN = 0x00FF00,
    BLUE = 0x0000FF,

    CYAN = 0x00FFFF,
    MAGENTA = 0xFF00FF,
    YELLOW = 0xFFFF00,
};

std::array<uint8_t, 3> hex_to_rgb(uint32_t hex){
    std::array<uint8_t, 3> rgb;

    rgb[0] = (hex & RED) >> 4*sizeof(uint32_t);
    rgb[1] = (hex & GREEN) >> 2*sizeof(uint32_t);
    rgb[2] = hex & BLUE;

    return rgb;
}

void test(){
}

#endif