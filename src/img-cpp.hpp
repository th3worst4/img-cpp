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


#ifdef IMG_CPP

struct Canvas{
    size_t w, h;
    size_t size;
    u_int8_t *data = NULL;

    Canvas(size_t w, size_t h);


    void save(std::string name);
};

Canvas::Canvas(size_t w, size_t h): w(w), h(h){
    size = w*h*3;
    data = new u_int8_t[size];
    std::fill(data, data+size, 255);
}

void Canvas::save(std::string name){

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

std::array<u_int8_t, 3> hex_to_rgb(u_int32_t hex){
    std::array<u_int8_t, 3> rgb;

    rgb[0] = (hex & RED) >> 4*sizeof(u_int32_t);
    rgb[1] = (hex & GREEN) >> 2*sizeof(u_int32_t);
    rgb[2] = hex & BLUE;

    return rgb;
}

void test(){
}

#endif