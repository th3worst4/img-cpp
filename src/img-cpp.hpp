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
#include <istream>
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
   /**
    * @brief Defines some basic hexadecimal colors to be used along the code
    *  
    * @returns uint32_t color value 
    *
    * @note Colors: white, black, pure red, pure green, pure blue, cyan, magenta, yellow.
    * 
   */
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

#endif

    /**
     * @brief Converts an uint32_t hex color code to RGB values.
     * The values are stored on a standard array.
     * 
     * @returns A std::array of size 3 where {R, G, B}
     * 
     * @note The type of elements on the array are uint8_t
     * 
    */
std::array<uint8_t, 3> hex_to_rgb(uint32_t hex){
    std::array<uint8_t, 3> rgb;

    rgb[0] = (hex & 0xFF0000) >> 4*sizeof(uint32_t);
    rgb[1] = (hex & 0x00FF00) >> 2*sizeof(uint32_t);
    rgb[2] = hex & 0x0000FF;

    for(uint8_t value : rgb){
        if(value > 255) IMG_CPPERRORCALL(CONV_ERR);
    }

    return rgb;
}

    /**
     * @brief Line object, connects two points with a line
     * 
     * @param points (x0, y0) --- (x, y)
     * @param line_width line width in pixels
     * @param hex_color uint32_t hex value for the color
     * 
     * @warning x0 must be less or equal x
     * 
     * @note Has default constructor
     * 
     */
typedef struct line{
    size_t x0, y0, x, y;
    size_t line_width;
    uint32_t hex_color;

    line(size_t x0, size_t y0, size_t x, size_t y, size_t line_width, uint32_t hex_color);
}line;

line::line(size_t x0, size_t y0, size_t x, size_t y, size_t lw, uint32_t hexc): x0(x0), y0(y0), x(x), y(y), line_width(lw), hex_color(hexc){
}

    /**
     * @brief Circunference object, creates a circunference
     * 
     * @param center (x0, y0)
     * @param radius radius length in pixels
     * 
     * @note The default constructor only requires the fundamental information about the circunference.
     * That is the center and the radius. To customize the color, for example, you need to call methods, like fill or stroke.
     * 
     */
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

    /**
     * @brief Canvas are the basic structure of all this library,
     * everything is supposed to be done on a canvas
     * 
     * @param w: width in pixels of the canvas
     * @param h: height in pixels of the canvas
     * @param size: size of the data array of the canvas, it's defined as w*h*3
     * @param data: uint8_t pointer that contains the data of all canvas' pixels
     * 
     * @note Has default constructor which requires only the width and size
     * 
     */
typedef struct Canvas{
    size_t w, h;
    size_t size;
    uint8_t *data = NULL;

    Canvas(size_t w, size_t h);
    Canvas(const char* name);

    int saveppm(const char* name);
    int copy(Canvas &out);

    int change_pixel(size_t x, size_t y, uint32_t hex_color);
    int line(struct line &l);
    int circunference(struct circ &c);
    
}Canvas;

    /**
     * @brief Default canvas constructor. Initializes an empty canvas
     * 
     * @param w canvas width
     * @param h canvas height
     * 
     */
Canvas::Canvas(size_t w, size_t h): w(w), h(h){
    /*
        Initialize a canvas object.
        Canvas are made to draw on them
    */
    size = w*h*3;
    data = new uint8_t[size];
    std::fill(data, data+size, 0xAAAAAA);
}

    /**
     * @brief Reads a .ppm file and store the information on a new canvas object
     * 
     * @param name or relative path of the file
     * 
     */
Canvas::Canvas(const char* name){
    std::fstream canvas(name, std::ios::in ^ std::ios::out);

    if(!canvas.is_open()) IMG_CPPERRORCALL(OPEN_ERR);
    std::string buffer;

    getline(canvas, buffer);

    if(buffer != "P3") IMG_CPP(OPEN_ERR);

    buffer.clear();
    canvas>>buffer;
    w = stoul(buffer);

    buffer.clear();
    canvas>>buffer;
    h = stoul(buffer);

    size = h*w*3;
    data = new uint8_t[size];

    buffer.clear();
    canvas>>buffer;
    if(stoi(buffer) != 255) IMG_CPP(OPEN_ERR);

    for(int i = 0; i < size; i++){
        buffer.clear();
        canvas>>buffer;
        *(data+i) = stoi(buffer);
    }
}

    /**
     * @brief Saves the created canvas on local disk
     * 
     * @param name: file name of the saved file
     * 
     * @returns 0 if successfull
     * 
    */
int Canvas::saveppm(const char* name){
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

    /**
     *  @brief Copy the canvas data from one canvas to another.
     * 
     *  @returns 0 if successful.
     * 
     */
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

    /**
     * @brief Change one and only pixel of the canvas. It's the basic idea for
     * all other drawing functions, still you can call it alone.
     * 
     * @param x position of the pixel 
     * @param y position of the pixel
     * @param hex_color uint32_t new hexadecimal color
     * 
     * @return 0 if successfull
     * 
     */
int Canvas::change_pixel(size_t x, size_t y, uint32_t hex_color){
    std::array<uint8_t, 3> rgb;
    rgb = hex_to_rgb(hex_color);

    for(int i = 0; i < 3; i++){
        *(data+x*3+y*w*3+i) = rgb[i];
    }

    return NO_ERR;
}

    /**
     *  @brief Creates a line based upon a line object
     * 
     *  @param line see typedef struct line for more info
     *  @returns 0 if successful
     * 
     *  @warning Line stroke needs improvements
     * 
     */
int Canvas::line(struct line &l){
    if(l.x0 > w || l.x > w || l.x0 > l.x) IMG_CPPERRORCALL(OUT_BOUND_ERR);
    if(l.y0 > h || l.y > h) IMG_CPPERRORCALL(OUT_BOUND_ERR);

    if(l.x != l.x0){
        float m = ((long)l.y0 - (long)l.y)/((long)l.x0 - (long)l.x);

        for(int i = 0; i <= abs(l.x - l.x0); i++){
            change_pixel(l.x0 + i, l.y0 + m*i, l.hex_color);
        }
    }

    return NO_ERR;
}

    /**
     * @brief Creates a circle based upon a circ object
     * @param circ See typedef struct circ for more info
     * 
     * @returns 0 if successful
     * 
     * @warning Circunference stroke needs improvements
     * 
    */
int Canvas::circunference(struct circ &c){
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