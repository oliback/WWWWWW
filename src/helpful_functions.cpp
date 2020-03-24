#include "helpful_functions.h"
#include <stdio.h>
#include <cstdarg>

void
Help_func::dbg_printf(const char* fmt, ...)
{
    #ifdef DEBUG
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    #endif
}
