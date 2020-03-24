#ifndef MAP_POINT_H
#define MAP_POINT_H

#include "SDL/SDL.h"
#include "block.h"

class MapPoint : public Block
{
private:
    int zone_x;
    int zone_y;

public:
    MapPoint(int x, int y);
    ~MapPoint();

    int get_zone_x();
    int get_zone_y();

    void set_zone_x(int x);
    void set_zone_y(int y);

    void load_spawn_texture();
    void load_goal_texture();

    virtual void apply_surface(SDL_Surface* dest);
};
#endif
