#ifndef BLOCK_H
#define BLOCK_H

#include "SDL/SDL.h"
#include "rect.h"

class Block : public Rect
{
protected:
    SDL_Surface *surface;
    bool fatal;
    int alpha;

public:
    Block();
    ~Block();
    Block(int x, int y, int width = 0, int height = 0);

    void set_color(int r, int g, int b);
    virtual void apply_surface(SDL_Surface* dest);

    bool falling_into(int y_chk, int h_chk);
    bool sliding_into(int x_chk, int w_chk);
    bool is_fatal();

    int get_surface_width();
    int get_surface_height();
    int get_alpha();

    /* return the outer y value relative to y_chk */
    int get_outer_y(int y_chk);
    /* return the outer x value relative to x_chk */
    int get_outer_x(int x_chk);

    void set_alpha(int alpha);
};

#endif
