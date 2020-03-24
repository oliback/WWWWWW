#include "widgets.h"
#include "wx_app.h"

WidgetApp* Widgets::widget_app;
int Widgets::argc;
char** Widgets::argv;
bool Widgets::is_init;

Widgets::Widgets()
{

}

Widgets::~Widgets()
{

}

//static
void
Widgets::init()
{
    if (!is_init) {
        argc = 0;
        argv = new char*[argc];
        widget_app = new WidgetApp();
        wxApp::SetInstance(widget_app);

        wxEntryStart(argc, argv);

        is_init = true;
    }
}

void
Widgets::cleanup()
{
    /* TODO: do some proper cleanup. */
    //delete [] argv;

    //wxEntryCleanup();

}

void
Widgets::color_picker(Help_func::Color& color, DIALOG_TYPE dialog_type)
{
    char* tmp = NULL;
    widget_app->OnInit(color, dialog_type, &tmp);
}

void
Widgets::file_dialog(char** path, DIALOG_TYPE dialog_type)
{
    Help_func::Color tmp;
    widget_app->OnInit(tmp, dialog_type, path);
}
