#include "spikes.h"
#include "sdl_functions.h"
#include "SDL/SDL_gfxPrimitives.h"

SDL_Surface* Spikes::spike_east;
SDL_Surface* Spikes::spike_west;
SDL_Surface* Spikes::spike_north;
SDL_Surface* Spikes::spike_south;

Spikes::Spikes(int x, int y, int width, int height, int dir) : Block(x, y, width, height)
{
    /* load the static textures if they have not yet been loaded */
    if (spike_east == NULL) {
        Spikes::spike_east = sdl_func::load_image("data/textures/spike_east.png");
    }

    if (spike_west == NULL) {
        spike_west = sdl_func::load_image("data/textures/spike_west.png");
    }

    if (spike_north == NULL) {
        spike_north = sdl_func::load_image("data/textures/spike_north.png");
    }

    if (spike_south == NULL) {
        spike_south = sdl_func::load_image("data/textures/spike_south.png");
    }

    /* set up the polygon */
    update();

    this->dir = dir;

    update_texture();

    this->fatal = true;
}

Spikes::~Spikes()
{
    if (spike_north != NULL) {
        SDL_FreeSurface(spike_north);
        spike_north = NULL;
    }

    if (spike_south != NULL) {
        SDL_FreeSurface(spike_south);
        spike_south = NULL;
    }

    if (spike_west != NULL) {
        SDL_FreeSurface(spike_west);
        spike_west = NULL;
    }

    if (spike_east != NULL) {
        SDL_FreeSurface(spike_east);
        spike_east = NULL;
    }
}

void
Spikes::update_texture()
{
    /* The old solution with static textures was nice 
     * even though it had its problems */
    if (surface != NULL) {
        SDL_FreeSurface(surface);
    }

    switch (dir) {
        case EAST:
            //surface = spike_east;
            surface = sdl_func::load_image("data/textures/spike_east.png");
            break;
        case WEST:
            //surface = spike_west;
            surface = sdl_func::load_image("data/textures/spike_west.png");
            break;
        case NORTH:
            //surface = spike_north;
            surface = sdl_func::load_image("data/textures/spike_north.png");
            break;
        case SOUTH:
            surface = spike_south;
            surface = sdl_func::load_image("data/textures/spike_south.png");
            break;
    }
}

void
Spikes::update()
{
    polygon_x[0] = x;           polygon_y[0] = y;
    polygon_x[1] = x + width;   polygon_y[1] = y;
    polygon_x[2] = x + width;   polygon_y[2] = y + height;
    polygon_x[3] = x;           polygon_y[3] = y + height;
}

int
Spikes::get_dir()
{
    return dir;
}

void
Spikes::rotate()
{
    /* TODO: constant? can only be 4 directions */
    dir = (dir + 1) % 4;
    update_texture();
}

void
Spikes::apply_surface(SDL_Surface* dest)
{
    if (surface == NULL) {
        printf("surface in Spikes was NULL\n");
        SDL_Quit();
    } else {
        /* Need to offset the texture to repeat 
         * correctly on the surface */
        int surface_width = get_surface_width();
        int surface_height = get_surface_height();

        int texture_offset_x = x % surface_width;
        int texture_offset_y = surface_height - y % surface_height;

        /* update the polygon */
        update();

        texturedPolygon(dest, polygon_x, polygon_y, POLY_VERT_NUM, surface,
                texture_offset_x, texture_offset_y);
    }
}
