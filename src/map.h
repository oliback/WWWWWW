#ifndef MAP_H
#define MAP_H

#include <vector>

#include "SDL/SDL.h"
#include "rect.h"
#include "block.h"
#include "zone.h"
#include "iomap.h"
#include "map_point.h"

/**
 * The `Map' is the main map handler
 * It takes care of the Zones
 **/

#define X_MAX 16

class IOMap;

class Map
{
    friend class IOMap;

private:
    int x, y;

    MapPoint* spawn;
    MapPoint* goal;
    std::vector<Zone*> zones;
    Zone *current_zone;

    IOMap *io;

    bool change_coords_relative(int& new_x, int& new_y, char dir);
public:
    typedef enum ZONE_CODE {
        SAFE, OUT_OF_BOUNDS, BLOCKED, ERROR
    } ZONE_CODE;

    Map();
    ~Map();

    void new_zones();
    void load_file(const char* path);
    bool copy_file(const char* src, const char* dest);
    bool save_to_file(const char* path);

    void draw_current_zone(SDL_Surface* dest);
    void draw_spawn(SDL_Surface* dest);
    std::vector<Block*> block_overlap_checks(Rect* r);
    std::vector<Spikes*> spikes_overlap_checks(Rect* r);

    ZONE_CODE set_zone_relative(Rect* player_rect, char dir);
    ZONE_CODE set_zone(Rect* player_rect, int new_x, int new_y);

    int get_current_zone_x();
    int get_current_zone_y();
    bool force_zone(int new_x, int new_y);
    bool force_zone_relative(char dir);
    void add_new_zone_relative(char dir);
    
    void add_block_to_current(Block* block);
    void add_spikes_to_current(Spikes* spikes);
    void print_xy();
    void create_spawn(int x=0, int y=0);
    void create_goal(int x=0, int y=0);
    void replace_spawn(MapPoint* new_spawn);
    void replace_goal(MapPoint* new_goal);
    void reset_to_spawn_zone();

    bool zone_exists(int x_chk, int y_chk);
    bool goal_check(Rect* rect);
    bool is_spawn_zone(Zone* zone);
    bool is_spawn_zone(int x_chk, int y_chk);
    bool is_goal_zone(Zone* zone);
    bool is_goal_zone(int x_chk, int y_chk);
    int get_spawn_x();
    int get_spawn_y();
};

#endif
