#ifndef WIDGETS_H
#define WIDGETS_H

#include "helpful_functions.h"

class WidgetApp;

typedef enum DIALOG_TYPE
{
    COLOR_PICKER,
    SAVE_FILE,
    LOAD_FILE
} DIALOG_TYPE;


class Widgets
{
private:
    static WidgetApp* widget_app;
    static int argc;
    static char** argv;
    static bool is_init;

public:
    Widgets();
    ~Widgets();

    void init();
    void cleanup();
    void color_picker(Help_func::Color& color, DIALOG_TYPE dialog_type);
    void file_dialog(char** path, DIALOG_TYPE dialog_type);
};

#endif
