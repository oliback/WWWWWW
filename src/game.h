#ifndef GAME_H
#define GAME_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "player.h"
#include "map.h"
#include "mini_map.h"
#include "button.h"
#include "map_editor.h"
#include "widgets.h"
#include "helpful_functions.h"

class Game
{
    private:
    typedef enum {
        MAIN_MENU, PLAYING, LOST, EDITING_MAP, DEV, WON
    } GameState;

    static const int FRAMES_PER_SECOND = 30;
    static const int MINI_MAP_X_BLOCKS = 3;
    static const int MINI_MAP_Y_BLOCKS = 3;
    //The surfaces
    SDL_Surface* background;
    SDL_Surface* screen;
    SDL_Surface* game_over_screen;
    SDL_Surface* win_screen;
    SDL_Surface* main_menu_screen;
    TTF_Font* font;
    Button* retry_button;
    Button* restart_button;
    Button* quit_button;
    Button* start_button;
    Button* load_map_button;
    Button* map_editor_button;
    Button* quit_main_button;

    Player* player;
    Map* map;
    MiniMap* mini_map;
    MapEditor* map_editor;
    Widgets* widgets;

    bool dev_mode;
    bool cap; //Whether or not to cap the frame rate
    GameState game_state;

    bool collision_detection();
    bool handle_player_events(SDL_Event &event);
    bool handle_mouse_events(SDL_Event &event);
    bool zone_control();
    bool load_map();

    void main_menu();
    void win();
    void game_over();

    public:
    Game();
    bool init();
    bool load_files();
    bool game_loop();
    ~Game();
};

#endif
