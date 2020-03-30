#ifndef HELP_FUNC_H
#define HELP_FUNC_H

class Help_func
{
private:
    static bool dbg;
public:
    typedef struct {
        int r, g, b;
    } Color;

    static void set_debug(bool _debug);
    static void dbg_printf(const char* fmt, ...);
};

#endif
