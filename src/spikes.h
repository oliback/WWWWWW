#ifndef SPIKES_H
#define SPIKES_H

#include "SDL/SDL.h"
#include "block.h"

#define POLY_VERT_NUM 4

class Spikes : public Block
{
protected:
    Sint16 polygon_x[POLY_VERT_NUM];
    Sint16 polygon_y[POLY_VERT_NUM];

    static SDL_Surface* spike_east;
    static SDL_Surface* spike_west;
    static SDL_Surface* spike_north;
    static SDL_Surface* spike_south;

    /* base class' SDL_Surface is used as the local texture surface */
    int dir;

    void update_texture();
    void update();

public:
    Spikes(int x, int y, int width, int height, int dir = SOUTH);
    ~Spikes();

    int get_dir();

    void rotate();
    void apply_surface(SDL_Surface* dest);

    typedef enum {
        EAST = 0, WEST = 1, NORTH = 2, SOUTH = 3
    } direction;
};
#endif
