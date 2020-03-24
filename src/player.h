#ifndef PLAYER_H
#define PLAYER_H

#include "SDL/SDL.h"
#include "rect.h"

#define STARTING_HEALTH 100
#define WALKING_ANIMATION_FRAMES 3
#define SPRITE_WIDTH 44
#define SPRITE_HEIGHT 45

class Player
{
    private:
    int x_start, y_start, x, y, old_x, old_y, width, height, health;
    
    char dir, old_dir, gravity, old_gravity;
    bool dir_set;
    SDL_Surface *sprite_left, *sprite_right, *sprite_left_flipped, *sprite_right_flipped;
    SDL_Rect rect_sprite;

    public:
    bool neutral_gravity;

    Player();
    bool is_dir_set();
    bool is_grav_neutral();
    bool set_image(const char *filename_left, const char *filename_right,
            const char *filename_left_flipped, const char *filename_right_flipped);

    char get_dir();
    char get_gravity();

    int get_x();
    int get_y();
    int get_old_x();
    int get_old_y();
    int get_width();
    int get_height();
    Rect* get_rect();

    void reset();
    void set_x(int x);
    void set_y(int y);
    void set_x_after_crash(int x);
    void set_y_after_crash(int y);
    void set_x_start(int x_start);
    void set_y_start(int y_start);
    void draw(SDL_Surface *context);
    /* returns true if new gravity was different from current gravity. */
    void change_dir(char dir);
    bool change_gravity(char gravity);
    void animate();
    void update();

    void restore_x();
    void restore_y();

    ~Player();
};

#endif
