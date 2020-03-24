#ifndef MINI_MAP_H
#define MINI_MAP_H

#include "SDL/SDL.h"
#include "helpful_functions.h"
#include "block.h"
#include "map.h"

class MiniMap
{
private:
    int x_block_count;
    int y_block_count;
    int zone_block_count;

    Help_func::Color base_color;
    Help_func::Color zone_color;
    Help_func::Color player_color;
    Help_func::Color goal_color;

    Block* base_block;
    Block** zone_blocks;
    Map* map;

    void set_block_off(int x, int y);
    void set_block_on(int x, int y);
    
public:
    MiniMap(int x_block_count, int y_block_count, Map* map);
    ~MiniMap();

    void update();
    void draw(SDL_Surface* surface);
};
#endif
