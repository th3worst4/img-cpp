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
#include <cmath>


#ifdef IMG_CPP

#define IMG_CPPERRORCALL(ERR_VAL) {std::cout<<"(img-cpp) ERROR CODE: "<<ERR_VAL<<std::endl; exit(ERR_VAL);}

#define IMG_CPPMIN(x, y) (((x) < (y)) ? (x) : (y))

enum ERR{
    
    //Some error codes to be used on this source file
    
    NO_ERR = 0,
    OPEN_ERR = 1,
    COPY_ERR = 2,
    CONV_ERR = 3,
    OUT_BOUND_ERR = 4,
};

enum HEX_COLOR{
    
    //Basic hex colors defined to be used without declaration on main code
    
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
    /*
        Converts a hex color code to RGB values,
        the values are stored on an array,
        this function returns a standard array
    */
    std::array<uint8_t, 3> rgb;

    rgb[0] = (hex & RED) >> 4*sizeof(uint32_t);
    rgb[1] = (hex & GREEN) >> 2*sizeof(uint32_t);
    rgb[2] = hex & BLUE;

    for(uint8_t value : rgb){
        if(value > 255) IMG_CPPERRORCALL(CONV_ERR);
    }

    return rgb;
}

struct Canvas{
    size_t w, h;
    size_t size;
    uint8_t *data = NULL;

    Canvas(size_t w, size_t h);

    int saveppm(const char* name);
    int copy(Canvas &out);
    int change_pixel(size_t x, size_t y, u_int32_t hex_color);
    int line(size_t x0, size_t y0, size_t x, size_t y, size_t line_width, uint32_t hex_color);
};

Canvas::Canvas(size_t w, size_t h): w(w), h(h){
    size = w*h*3;
    data = new uint8_t[size];
    std::fill(data, data+size, 0);
}

int Canvas::saveppm(const char* name){
    /*
        something is wrong with this save function.
        idk why, but saving two canvas in sequence makes the last saved
        canvas to not be properly saved
    */
    std::fstream canvas(name, std::ios::out);

    if(!canvas.is_open()) IMG_CPPERRORCALL(OPEN_ERR);

    canvas << "P3\n" << w << ' ' << h << "\n255\n";

    for(int i = 0; i < size; i++){
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
        IMG_CPPERRORCALL(COPY_ERR);
    }

    for(int i = 0; i < size; i++){
        *(out.data+i) = *(data+i);
    }

    return NO_ERR;
}

int Canvas::change_pixel(size_t x, size_t y, uint32_t hex_color){
    /*
        This function is supposed to change just one pixel of the canvas.
        Kindda useless, but as I'm working on a general purpose use library,
        this may be usefull for some thing.
    */
    --x; --y;
    std::array<uint8_t, 3> rgb;
    rgb = hex_to_rgb(hex_color);

    for(int i = 0; i < 3; i++){
        *(data+x*3+y*w*3+i) = rgb[i];
    }

    return NO_ERR;
}

int Canvas::line(size_t x0, size_t y0, size_t x, size_t y, size_t line_width, uint32_t hex_color){ 
    if((x0 > w || x > w) || x0 > x) IMG_CPPERRORCALL(OUT_BOUND_ERR);
    if((y0 > h || y > h) || y0 > y) IMG_CPPERRORCALL(OUT_BOUND_ERR);

    float m = (y0-y)/(x0-x);
    size_t dx = abs(x0-x);

    for(int i = 0; i <= dx; i++){
        size_t sx = x0 + i;
        size_t sy = y0 + m*i;

        change_pixel(sx, sy, hex_color);
        for(int j = 0; j < line_width; j++){
            change_pixel(sx, sy+j, hex_color);
            change_pixel(sx, sy-j, hex_color);
        }
    }
 
    return NO_ERR;
}

#endif