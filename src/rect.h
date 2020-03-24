#ifndef RECTOBJECT_H
#define RECTOBJECT_H

#include "helpful_functions.h"

class Rect
{
protected:
    int x, y, x_origin, y_origin, width, height;
    Help_func::Color color;

public:
    Rect(int x = 0, int y = 0, int width = 0, int height = 0, int r = 0, int g = 0, int b = 0);
    virtual ~Rect();

    bool overlap(Rect *r);
    virtual bool boundary_chk(int x_chk, int y_chk);

    int get_x();
    int get_y();
    int get_width();
    int get_height();
    int get_r();
    int get_g();
    int get_b();
    int get_x_origin();
    int get_y_origin();

    void restore_x();
    void restore_y();
    void set_x(int x);
    void set_y(int y);
    void set_width(int w);
    void set_height(int h);
    void set_r(int r);
    void set_g(int g);
    void set_b(int b);
};

#endif
