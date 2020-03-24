#include "wx_app.h"

BEGIN_EVENT_TABLE(WidgetFrame, wxFrame)
    EVT_MENU(ID_Quit, WidgetFrame::OnQuit)
END_EVENT_TABLE()

WidgetFrame::WidgetFrame() : wxFrame(NULL, wxID_ANY,  wxT("Hello wxWidgets"), wxPoint(50,50), wxSize(800,600))
{

}

bool
WidgetApp::OnInit(Help_func::Color& color, DIALOG_TYPE dialog_type, char** path)
{
    Help_func::dbg_printf("before new WidgetFrame()\n");
    WidgetFrame* frame = new WidgetFrame();

    if (dialog_type == COLOR_PICKER) {
        Help_func::dbg_printf("before frame->color_dialog()\n");
        frame->color_dialog(color, dialog_type);
    } else if (dialog_type == SAVE_FILE || dialog_type == LOAD_FILE){
        Help_func::dbg_printf("before frame->file_dialog()\n");
        frame->file_dialog(path, dialog_type);
    }

    Help_func::dbg_printf("before frame->Show()\n");
    frame->Show(true);
    Help_func::dbg_printf("before frame->Close()\n");
    frame->Close(TRUE);
    Help_func::dbg_printf("before SetTopWindow(frame)\n");
    SetTopWindow(frame);
    Help_func::dbg_printf("before frame->Destroy()\n");
    frame->Destroy();
    Help_func::dbg_printf("after frame->Destroy()\n");

    return true;
}

void
WidgetFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void
WidgetFrame::color_dialog(Help_func::Color& color, DIALOG_TYPE dialog_type)
{
    wxColourData data;
    data.SetChooseFull(false);
    for (int i = 0; i < 16; ++i)
    {
        wxColour colour(i*16, i*16, i*16);
        data.SetCustomColour(i, colour);
    }

    wxColourDialog dialog(this, &data);
    if (dialog.ShowModal() == wxID_OK) {
        wxColourData retData = dialog.GetColourData();
        wxColour col = retData.GetColour();
        color.r = col.Red();
        color.g = col.Green();
        color.b = col.Blue();

        Help_func::dbg_printf("color.r=%d\ncolor.g=%d\ncolor.b=%d\n", color.r, color.g, color.g);
    }

    dialog.Destroy();
}

void
WidgetFrame::file_dialog(char** path, DIALOG_TYPE dialog_type)
{
    Help_func::dbg_printf("before wxFileDialog()\n");
    wxFileDialog* dialog;

    if (dialog_type == LOAD_FILE) {
        dialog = new wxFileDialog(this, _("Load map"), wxEmptyString, wxEmptyString,
                _("XML files (*.xml)|*.xml"), wxFD_OPEN);
    } else if (dialog_type == SAVE_FILE) {
        dialog = new wxFileDialog(this, _("Save map"), wxEmptyString, wxEmptyString,
                _("XML files (*.xml)|*.xml"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    } else {
        Help_func::dbg_printf("widgets: invalid dialog_type\n");
        return;
    }

    Help_func::dbg_printf("before SetDirectory\n");
    dialog->SetDirectory(_("data/maps/"));

    if (dialog->ShowModal() == wxID_OK) {
        wxString wx_path;
        wx_path = dialog->GetPath();

        *path = new char[wx_path.length() + 1];
        strcpy(*path, (const char*)wx_path.mb_str(wxConvUTF8));

        Help_func::dbg_printf("in file_dialog(), *path = %s\n", *path);

        wx_path.Clear();
    }

    dialog->Destroy();
}
