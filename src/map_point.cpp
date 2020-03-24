#include "map_point.h"
#include "sdl_functions.h"

MapPoint::MapPoint(int x, int y) : Block(x, y, 0, 0)
{
    surface = NULL;
}

MapPoint::~MapPoint()
{

}

int
MapPoint::get_zone_x()
{
    return this->zone_x;
}

int
MapPoint::get_zone_y()
{
    return this->zone_y;
}

void
MapPoint::set_zone_x(int x)
{
    this->zone_x = x;
}

void
MapPoint::set_zone_y(int y)
{
    this->zone_y = y;
}

void
MapPoint::load_spawn_texture()
{
    if (surface != NULL) {
        SDL_FreeSurface(surface);
    }

    surface = sdl_func::load_image("data/textures/spawn.png");
}

void
MapPoint::load_goal_texture()
{
    if (surface != NULL) {
        SDL_FreeSurface(surface);
    }

    surface = sdl_func::load_image("data/textures/goal.png");
}

void
MapPoint::apply_surface(SDL_Surface* dest)
{
    if (surface != NULL) {
        sdl_func::apply_surface(x, y, surface, dest);
    }
}
