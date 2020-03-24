#include "block.h"
#include "sdl_functions.h"

Block::Block() : Rect()
{
    surface = NULL;
    fatal = false;

    /* standard alpha */
    this->alpha = SDL_ALPHA_OPAQUE;
}

Block::~Block()
{
    SDL_FreeSurface(surface);
}

Block::Block(int x, int y, int width, int height) : Rect(x, y, width, height)
{
    surface = NULL;
    fatal = false;

    /* standard alpha */
    this->alpha = SDL_ALPHA_OPAQUE;
}

void
Block::set_color(int r, int g, int b)
{
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    /* store color */
    this->color.r = r;
    this->color.g = g;
    this->color.b = b;

    /* check if null, if not freesurface? */
    if (surface != NULL) {
        SDL_FreeSurface(surface);
        surface = NULL;
    }

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height,
            32, rmask, gmask, bmask, amask);
        SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, r, g, b, this->alpha));
}

void
Block::apply_surface(SDL_Surface *dest)
{
    if (surface->w != this->width || surface->h != this->height) {
        set_color(color.r, color.g, color.b);
    }

    sdl_func::apply_surface(x, y, surface, dest);
}

bool
Block::falling_into(int y_chk, int h_chk)
{
    return !(y_chk < y + height && y_chk + h_chk > y);
}

bool
Block::sliding_into(int x_chk, int w_chk)
{
    return !(x_chk < x + width && x_chk + w_chk > x);
}

bool
Block::is_fatal()
{
    return fatal;
}

int
Block::get_surface_width()
{
    return surface->w;
}

int
Block::get_surface_height()
{
    return surface->h;
}

int
Block::get_alpha()
{
    return this->alpha;
}

int
Block::get_outer_y(int y_chk)
{
    if (y_chk < y + height / 2)
        return y;
    else
        return y + height;
}

int
Block::get_outer_x(int x_chk)
{
    if (x_chk < x + width/2) {
        return x;
    } else {
        return x + width;
    }
}

void
Block::set_alpha(int alpha)
{
    this->alpha = alpha;

    if (surface != NULL) {
        set_color(color.r, color.g, color.b);
    }
}
