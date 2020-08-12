#include "player.h"
#include "sdl_functions.h"

Player::Player()
{
    health = STARTING_HEALTH;
    width = height = 0;
    sprite_left = NULL;
    sprite_right = NULL;
    sprite_left_flipped = NULL;
    sprite_right_flipped = NULL;
    neutral_gravity = false;
    gravity = old_gravity = '\0';
    /* setting the width to 8.. want collission to treat player 
     * like a block */
    width = 8;
    height = 45;

    x_start = 0;
    y_start = 0;

    dir = '\0';
    dir_set = false;
}

bool
Player::is_dir_set()
{
    return dir_set;
}

bool
Player::is_grav_neutral()
{
    return neutral_gravity;
}

/* filename_flipped should be the flipped version of filename */
bool
Player::set_image(const char *filename_left, const char *filename_right,
        const char *filename_left_flipped, const char *filename_right_flipped)
{
    sprite_left = sdl_func::load_image(filename_left);
    sprite_right = sdl_func::load_image(filename_right);
    sprite_left_flipped = sdl_func::load_image(filename_left_flipped);
    sprite_right_flipped = sdl_func::load_image(filename_right_flipped);

    if (sprite_left == NULL || sprite_right == NULL ||
            sprite_left_flipped == NULL || sprite_right_flipped == NULL) {
        return false;
    }

    rect_sprite.w = SPRITE_WIDTH;
    rect_sprite.h = SPRITE_HEIGHT;
    rect_sprite.x = 0;
    rect_sprite.y = 0;
    
    return true;
}

int
Player::get_x()
{
    return x;
}

int
Player::get_y()
{
    return y;
}

int
Player::get_old_x()
{
    return old_x;
}

int
Player::get_old_y()
{
    return old_y;
}

int
Player::get_width()
{
    return width;
}

int
Player::get_height()
{
    return height;
}

/* caller responsible for deleting */
Rect*
Player::get_rect()
{
    return (new Rect(x, y, width, height));
}

char
Player::get_dir()
{
    return dir;
}

char
Player::get_gravity()
{
    return gravity;
}

void
Player::reset()
{
    /* TODO: static way to do it, compensating for where the spawn point
     * is and how big it is. Would be better to retrieve these values */
    x = old_x = x_start + (int)(16/2);
    y = old_y = y_start - height + 16;
    health = STARTING_HEALTH;
    dir = '\0';
    dir_set = false;
    gravity = 'd';
}

void
Player::set_x(int x)
{
    this->x = x;
}

void
Player::set_y(int y)
{
    this->y = y;
}

/* TODO: needs safeguards? */
void
Player::set_x_after_crash(int x)
{
    /* TODO
     * not really sure about this. mostly an experiment.
     * Something needs to be done though... */
    int n = old_x < x ? -width : 0;
    this->x = x + n;
    old_x = this->x;
}

/* TODO: needs safeguards? */
void
Player::set_y_after_crash(int y)
{
    switch (gravity) {
        case 'd':
            this->y = y - height;
            break;
        case 'u':
            this->y = y;
            break;
    }

    /* TODO should make sure that these values should be/not be set */
    old_y = this->y;
}

void
Player::set_x_start(int x_start)
{
    this->x_start = x_start;
}

void
Player::set_y_start(int y_start)
{
    this->y_start = y_start;
}

void
Player::draw(SDL_Surface *context)
{
    if (this->gravity == 'u') {
        /* want to keep facing the same direction as old_dir */
        if (this->old_dir == 'r') {
            sdl_func::apply_surface(x - 18, y, sprite_right_flipped, context, &rect_sprite);
        } else {
            sdl_func::apply_surface(x - 18, y, sprite_left_flipped, context, &rect_sprite);
        }
    } else {
        if (this->old_dir == 'r') {
            sdl_func::apply_surface(x - 18, y, sprite_right, context, &rect_sprite);
        } else {
            sdl_func::apply_surface(x - 18, y, sprite_left, context, &rect_sprite);
        }
    }
}

void
Player::change_dir(char dir)
{
    switch (dir) {
        case 'l':
        case 'r':
            this->old_dir = dir;
            dir_set = true;
            break;
        default:
            dir_set = false;
    }

    this->dir = dir;
}

/* returns true if new gravity was different from current gravity. */
bool
Player::change_gravity(char gravity)
{
    /* TODO
     * invert the old y value because of gravity change */
    bool changed = (gravity != this->gravity);
    old_gravity = this->gravity;
    this->gravity = gravity;
    neutral_gravity = false;

    return changed;
}

void
Player::update()
{
    switch (dir) {
        case 'l':
            old_x = x;
            x -= X_SPEED;
            /* sprite animation */
            this->animate();
            break;
        case 'r':
            old_x = x;
            x += X_SPEED;
            this->animate();
            break;
    }

    /* TODO move old_y = y; outside of the if? or other way around with x*/
    if (!neutral_gravity) {
        old_y = y;

        switch (gravity) {
            case 'd':
                y += GRAVITY;
                break;
            case 'u':
                y -= GRAVITY;
                break;
        }
    }
}

void
Player::animate()
{
    /* TODO: store these constants in a nice manner */
    rect_sprite.x = (rect_sprite.x + 44) % 176;
}

void
Player::restore_x()
{
    x = old_x;
}

void
Player::restore_y()
{
    y = old_y;
}

Player::~Player()
{
    SDL_FreeSurface(sprite_left);
    SDL_FreeSurface(sprite_right);
    SDL_FreeSurface(sprite_left_flipped);
    SDL_FreeSurface(sprite_right_flipped);
}
