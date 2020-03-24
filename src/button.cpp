#include "button.h"
#include "sdl_functions.h"

Button::Button(int r, int g, int b) : Block()
{
    message = NULL;
    txt_color.r = r;
    txt_color.g = g;
    txt_color.b = b;
    x_offset = 2;
    click_count = -1;
    button_on = false;

    on_color.r = on_color.g = on_color.b = 0;
    off_color.r = off_color.g = off_color.b = 0;

    /* yellow for click */
    click_color.r = 255;
    click_color.g = 255;
    click_color.b = 0;
}

Button::Button(int x, int y, int width, int height, int r, int g, int b) : 
    Block(x, y, width, height)
{
    message = NULL;
    txt_color.r = r;
    txt_color.g = g;
    txt_color.b = b;
    x_offset = 5;
    y_offset = 10;
    click_count = -1;
    button_on = false;

    on_color.r = on_color.g = on_color.b = 0;
    off_color.r = off_color.g = off_color.b = 0;

    /* yellow for click */
    click_color.r = 255;
    click_color.g = 255;
    click_color.b = 0;
}

Button::~Button()
{
    SDL_FreeSurface(message);
}

bool
Button::set_message(TTF_Font *font, const char *str)
{
    message = TTF_RenderText_Solid(font, str, txt_color);

    if (message == NULL) {
        return false;
    }

    return true;
}

bool
Button::boundary_chk(int click_x, int click_y)
{
    bool clicked = false;

    clicked = Rect::boundary_chk(click_x, click_y);

    if (clicked) {
        click();
    }

    return clicked;
}

void
Button::apply_surface(SDL_Surface *dest)
{
    sdl_func::apply_surface(x, y, surface, dest);
    //place message in the middle of the block
    sdl_func::apply_surface(x + (width/2 - message->w/2),
            y + (height/2 - message->h/2), message, dest);

    if (click_count > -1) {
        click_count++;
        
        if (click_count >= click_iterations) {
            if (button_on) {
                set_color(on_color.r, on_color.g, on_color.b);
            } else {
                set_color(off_color.r, off_color.g, off_color.b);
            }

            click_count = -1;
        }
    }
}

void
Button::click()
{
    /* the button will change color for click_iterations draw cycles */
    click_count = 0;

    set_color(click_color.r, click_color.g, click_color.b);
}

void
Button::flip_on_off()
{
    if (click_count > -1) {
        /* want to show the clicked color first */
        button_on = !button_on;
        return;
    }

    if (button_on) {
        set_color(off_color.r, off_color.g, off_color.b);
        button_on = false;
    } else {
        set_color(on_color.r, on_color.g, on_color.b);
        button_on = true;
    }
}

void
Button::set_tag(int tag)
{
    this->tag = tag;
}

void
Button::set_off_color(int r, int g, int b)
{
    off_color.r = r;
    off_color.g = g;
    off_color.b = b;

    //base class has r, g, b vars that will be written
    if (!button_on) set_color(r, g, b);
}

void
Button::set_on_color(int r, int g, int b)
{
    on_color.r = r;
    on_color.g = g;
    on_color.b = b;

     if (button_on) set_color(r, g, b);
}

void
Button::set_text_color(int r, int g, int b)
{
    // doesn't really change the actual color, only stores it.
    txt_color.r = r;
    txt_color.g = g;
    txt_color.b = b;
}

int
Button::get_tag()
{
    return this->tag;
}
