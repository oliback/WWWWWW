#ifndef WX_APP_H
#define WX_APP_H

#include "wx/wx.h"
#include "wx/colordlg.h"
#include "wx/filedlg.h"
#include "wx/intl.h"
#include "helpful_functions.h"
#include "widgets.h"

class WidgetApp : public wxApp
{
public:
    virtual bool OnInit(Help_func::Color& color, DIALOG_TYPE dialog_type, char** path);
};

class WidgetFrame : public wxFrame
{
public:
    WidgetFrame();
    void OnQuit(wxCommandEvent& event);
    void color_dialog(Help_func::Color& color, DIALOG_TYPE dialog_type);
    void file_dialog(char** path, DIALOG_TYPE dialog_type);

    DECLARE_EVENT_TABLE()
};

enum
{
    ID_Quit = 1,
};

#endif
