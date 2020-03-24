#include "rect.h"

Rect::Rect(int x, int y, int width, int height, int r, int g, int b)
{
    this->x = x;
    this->y = y;
    this->x_origin = x;
    this->y_origin = y;
    this->width = width;
    this->height = height;
    this->color.r = r;
    this->color.g = g;
    this->color.b = b;
}

bool
Rect::overlap(Rect *r)
{
    return !(r->x >= x + width 
            || r->x + r->width <= x
            || r->y >= y + height
            || r->y + r->height <= y);
}

bool
Rect::boundary_chk(int x_chk, int y_chk)
{
    return ((x_chk >= x && x_chk <= x + width) &&
            (y_chk >= y && y_chk <= y + height));
}

int
Rect::get_x()
{
    return x;
}

int
Rect::get_y()
{
    return y;
}

int
Rect::get_width()
{
    return width;
}

int
Rect::get_height()
{
    return height;
}

int
Rect::get_r()
{
    return this->color.r;
}

int
Rect::get_g()
{
    return this->color.g;
}

int
Rect::get_b()
{
    return this->color.b;
}

int
Rect::get_x_origin()
{
    return this->x_origin;
}

int
Rect::get_y_origin()
{
    return this->y_origin;
}

void
Rect::restore_x()
{
    this->x = this->x_origin;
}

void
Rect::restore_y()
{
    this->y = this->y_origin;
}

void
Rect::set_x(int x)
{
    this->x = x;
}

void
Rect::set_y(int y)
{
    this->y = y;
}

void
Rect::set_width(int w)
{
    this->width = w;
}

void
Rect::set_height(int h)
{
    this->height = h;
}

void
Rect::set_r(int r)
{
    this->color.r = r;
}

void
Rect::set_g(int g)
{
    this->color.g = g;
}

void
Rect::set_b(int b)
{
    this->color.b = b;
}

Rect::~Rect()
{

}
