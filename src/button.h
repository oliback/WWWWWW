#ifndef BUTTON_H
#define BUTTON_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "block.h"
#include "helpful_functions.h"

class Button : public Block
{
    private:
    static const int click_iterations = 3;

    SDL_Surface *message;
    SDL_Color txt_color;

    int x_offset;
    int y_offset;
    int tag;
    int click_count;
    bool button_on;
    Help_func::Color on_color;
    Help_func::Color off_color;
    Help_func::Color click_color;

    public:
    Button(int r = 0, int g = 0, int b = 0);
    Button(int x, int y, int width = 0, int height = 0, int r = 0, int g = 0, int b = 0);

    ~Button();

    bool set_message(TTF_Font *font, const char *str);

    virtual bool boundary_chk(int click_x, int click_y);
    virtual void apply_surface(SDL_Surface *dest);
    /* should change color for a few draw cycles and then switch back */
    void click();
    /* change the color of the button until called again */
    void flip_on_off();
    void set_tag(int tag);
    void set_off_color(int r, int g, int b);
    void set_on_color(int r, int g, int b);
    void set_text_color(int r, int g, int b);
    /* overridden RectObjects boundary_chk */

    int get_tag();
};

#endif
