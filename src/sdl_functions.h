#ifndef SDL_FUNCTIONS_H
#define SDL_FUNCTIONS_H

#include "SDL/SDL.h"

// physics attributes
#define GRAVITY 10
#define X_SPEED 5
/* TODO: X_SPEED was previously 4 */

// screen attributes
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

class sdl_func
{
/* switched order of public private to allow private functions to use Color struct */
public:
    typedef struct {
        Uint8 r;
        Uint8 g;
        Uint8 b;
    } Color;

    static SDL_Surface* load_image(const char *filename);

    static void apply_surface(int x, int y, SDL_Surface* source,
            SDL_Surface* destination, SDL_Rect* clip = NULL);

    /* Bresenham's line algorithm 
     * http://stackoverflow.com/questions/11737988/how-to-draw-a-line-using-sdl-without-using-external-libraries */
    static void draw_line(SDL_Surface* surface, float x1, float y1, float x2, float y2, Color color);

private:
    static void set_pixel(SDL_Surface* surface, int x, int y, Color color);
};

#endif
