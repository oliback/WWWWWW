#include "helpful_functions.h"
#include <stdio.h>
#include <cstdarg>

bool
Help_func::dbg = false;

void
Help_func::set_debug(bool _debug)
{
    Help_func::dbg = _debug;
}

void
Help_func::dbg_printf(const char* fmt, ...)
{
    if (Help_func::dbg) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}