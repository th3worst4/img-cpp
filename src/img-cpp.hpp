/*
    The main goal of this project is to create a header only library that
    allows simple image creation/modification.

    The ideia of this whole thing was taken from the Tsoding channel;
    I thought it would be a nice challenge to me to reacreate his ideias
    but by my own
*/

//  todo:
//  circunference
//  rectangle
//  read function
//  polygon

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

#ifdef BASIC_HEX

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

#endif

std::array<uint8_t, 3> hex_to_rgb(uint32_t hex){
    /*
        Converts a hex color code to RGB values,
        the values are stored on an array,
        this function returns a standard cpp array
    */
    std::array<uint8_t, 3> rgb;

    rgb[0] = (hex & 0xff000) >> 4*sizeof(uint32_t);
    rgb[1] = (hex & 0x00ff00) >> 2*sizeof(uint32_t);
    rgb[2] = hex & 0x0000ff;

    for(uint8_t value : rgb){
        if(value > 255) IMG_CPPERRORCALL(CONV_ERR);
    }

    return rgb;
}

typedef struct line{
    size_t x0, y0, x, y;
    size_t line_width;
    uint32_t hex_color;

    line(size_t x0, size_t y0, size_t x, size_t y, size_t line_width, uint32_t hex_color);
}line;

line::line(size_t x0, size_t y0, size_t x, size_t y, size_t lw, uint32_t hexc): x0(x0), y0(y0), x(x), y(y), line_width(lw), hex_color(hexc){
}


typedef struct circ{
    int64_t x0, y0;
    size_t radius;    

    bool fill_state;
    uint32_t fill_color;

    bool stroke_state;
    size_t stroke_width;
    uint32_t stroke_color;

    circ(int64_t x, int64_t y, size_t radius);
    
    int fill(bool fs, uint32_t fc = 0xffffff);
    int stroke(bool s, size_t sw = 1, uint32_t sc = 0xffffff);

}circ;

circ::circ(int64_t x, int64_t y, size_t radius): x0(x), y0(y), radius(radius){
}

int circ::fill(bool fs, uint32_t fc){
    fill_state = fs;
    fill_color = fc;

    return NO_ERR;
}

int circ::stroke(bool s, size_t sw, uint32_t sc){
    stroke_state = s;
    stroke_width = sw;
    stroke_color = sc;

    return NO_ERR;
}

struct Canvas{
    size_t w, h;
    size_t size;
    uint8_t *data = NULL;

    Canvas(size_t w, size_t h);

    int saveppm(const char* name);
    int copy(Canvas &out);
    int change_pixel(size_t x, size_t y, uint32_t hex_color);
    int line(struct line l);
    int circunference(struct circ c);
    
};

Canvas::Canvas(size_t w, size_t h): w(w), h(h){
    /*
        Initialize a canvas object.
        Canvas are made to draw on them
    */
    size = w*h*3;
    data = new uint8_t[size];
    std::fill(data, data+size, 0xAAAAAA);
}

int Canvas::saveppm(const char* name){
    /*
        Saves the created object by the path/name passed in name variable.
        Returns 0 if successful
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
    /*
        Copy the canvas data from one canvas to another.
        Returns 0 if successful
    */
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
        It's the basic idea for all other drawing functions, 
        still you can call it alone
        Returns 0 if successful
    */
    --x; --y;
    std::array<uint8_t, 3> rgb;
    rgb = hex_to_rgb(hex_color);

    for(int i = 0; i < 3; i++){
        *(data+x*3+y*w*3+i) = rgb[i];
    }

    return NO_ERR;
}

int Canvas::line(struct line l){
    /*
        Creates a line that connects (x0, y0) to (x, y).
        x0 must be less than x or it will evoque an out of bounds error
        Returns 0 if successful
    */
    if((l.x0 > w || l.x > w) || l.x0 > l.x) IMG_CPPERRORCALL(OUT_BOUND_ERR);
    if((l.y0 > h || l.y > h) || l.y0 > l.y) IMG_CPPERRORCALL(OUT_BOUND_ERR);

    if(l.x0 - l.x){
        float m = (l.y0-l.y)/(l.x0-l.x);
        size_t dx = abs(l.x0-l.x);
        for(int i = 0; i <= dx; i++){
            size_t sx = l.x0 + i;
            size_t sy = l.y0 + m*i;

            change_pixel(sx, sy, l.hex_color);
            for(int j = 0; j < l.line_width; j++){
                change_pixel(sx, sy+j, l.hex_color);
                change_pixel(sx, sy-j, l.hex_color);
            }
        }
    }else{
        size_t dy = abs(l.y0 - l.y);
        for(int i = 0; i < dy; i++){
            change_pixel(l.x, l.y0 + i, l.hex_color);
            for(int j = 0; j < l.line_width; j++){
                change_pixel(l.x+j, l.y0 + i, l.hex_color);
                change_pixel(l.x-j, l.y0 + i, l.hex_color);
            } 
        }
    }
 
    return NO_ERR;
}

int Canvas::circunference(struct circ c){
    if(c.x0 - c.radius > w || c.x0 + c.radius < 0) IMG_CPPERRORCALL(OUT_BOUND_ERR);
    if(c.y0 - c.radius > h || c.y0 + c.radius < 0) IMG_CPPERRORCALL(OUT_BOUND_ERR);

    if(c.fill_state){
        for(int x = c.x0 - c.radius; x < c.x0 + c.radius; x++){
            for(int y = c.y0 - c.radius; y < c.y0 + c.radius; y++){
                if(pow(x-c.x0, 2) + pow(y-c.y0, 2) <= pow(c.radius, 2)) change_pixel(x, y, c.fill_color);
            }
        }
    }

    if(c.stroke_state){
        for(int x = c.x0 - c.radius; x < c.x0 + c.radius; x++){
            for(int y = c.y0 - c.radius; y < c.y0 + c.radius; y++){
                if(pow(x-c.x0, 2) + pow(y-c.y0, 2) == pow(c.radius, 2)) change_pixel(x, y, c.stroke_color);
            }
        }
    }

    return NO_ERR;
}

#endif