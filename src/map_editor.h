#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include <vector>
#include <algorithm> // only for std::max, lol
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "block.h"
#include "button.h"
#include "map.h"
#include "mini_map.h"
#include "helpful_functions.h"
#include "widgets.h"

#define INCREMENT 16

class MapEditor
{
private:
    typedef enum {
        NONE, BLOCK, SPIKES, ROTATE, SPAWN, GOAL
    } Tool;

    enum {
        BLOCK_BUTTON, SPIKES_BUTTON, ROTATE_BUTTON,
        LEFT_ZONE_BUTTON, RIGHT_ZONE_BUTTON,
        DOWN_ZONE_BUTTON, UP_ZONE_BUTTON,
        SPAWN_BUTTON, GOAL_BUTTON,
        SNAP_BUTTON, COLOR_BUTTON,
        LOAD_BUTTON, SAVE_BUTTON
    };
    
    static const int MINI_MAP_X_BLOCKS = 7;
    static const int MINI_MAP_Y_BLOCKS = 7;
    static const int BUTTON_COUNT = 13;
    
    Widgets* widgets;
    Help_func::Color draw_color;
    bool snap_active;
    TTF_Font* font;
    Map* map;
    MiniMap* mini_map;
    Tool active_tool;
    Button* buttons[BUTTON_COUNT];
    Block* draw_block;

    int snap_to_grid(int val);
    void flip_button_color(int button_i);
    void draw_buttons(SDL_Surface* surface);

    int rand_color_component();
    void color_widget();
    void save_widget();
    void load_widget();

public:
    MapEditor();
    ~MapEditor();
    bool handle_user_events(SDL_Event &event);
    bool draw(SDL_Surface* screen);
};

#endif
