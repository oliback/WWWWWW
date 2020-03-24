#ifndef HELP_FUNC_H
#define HELP_FUNC_H

class Help_func
{
public:
    typedef struct {
        int r, g, b;
    } Color;

    static void dbg_printf(const char* fmt, ...);
};

#endif
