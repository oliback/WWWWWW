#include "sdl_functions.h"
#include "SDL/SDL_image.h"
#include <math.h>
#include <utility>

void
sdl_func::set_pixel(SDL_Surface* surface, int x, int y, Color color)
{
    if (surface == NULL) {
        printf("surface == NULL, sdl_func::set_pixel\n");
        return;
    }

    Uint32 *pixmem32;
    Uint32 final_color;

    final_color = SDL_MapRGB(surface->format, color.r, color.g, color.b);
  
    int new_y = y * surface->pitch / 4;
 
    pixmem32 = (Uint32*) surface->pixels + new_y + x;
    *pixmem32 = final_color; 
}

SDL_Surface*
sdl_func::load_image(const char *filename)
{
    SDL_Surface* loaded_image = NULL;

    SDL_Surface* optimized_image = NULL;

    loaded_image = IMG_Load(filename);

    if(loaded_image != NULL)
    {
        optimized_image = SDL_DisplayFormat(loaded_image);

        //Free the old surface
        SDL_FreeSurface(loaded_image);

        //If the surface was optimized
        if (optimized_image != NULL) {
            //Color key surface
            SDL_SetColorKey(optimized_image, SDL_SRCCOLORKEY, 
                    SDL_MapRGB(optimized_image->format, 0x9b, 0x21, 0xe8)); //purple
        }
    }

    //Return the optimized surface
    return optimized_image;
}

void
sdl_func::apply_surface(int x, int y, SDL_Surface* source,
        SDL_Surface* destination, SDL_Rect* clip)
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface(source, clip, destination, &offset);
}

/* Bresenham's line algorithm 
 * http://stackoverflow.com/questions/11737988/how-to-draw-a-line-using-sdl-without-using-external-libraries */
void
sdl_func::draw_line(SDL_Surface* surface, float x1, float y1, float x2, float y2, Color color)
{
    if (surface == NULL) {
        printf("surface == NULL in sdl_func::draw_line\n");
        return;
    }

    const bool steep = (fabs(y2 - y1) > fabs (x2 - x1));

    if (steep) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    
    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for (int x = (int)x1; x < maxX; ++x)
    {
        if (steep) {
            sdl_func::set_pixel(surface, y, x, color);
        } else {
            sdl_func::set_pixel(surface, x, y, color);
        }

        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}
