// 2025/7/25 12:23:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/GlobalFunctions.h"
#include "MainWindow.h"
#include "Controls/DraggedWindow.h"


class Dialog : public DraggedDialog
{
public:

    Dialog(const wxString &, const wxPoint & = wxDefaultPosition, const wxSize &size = wxDefaultSize);

    void ShowOnWindow(wxWindow *window);

protected:

    int CreateRadioButton(wxWindow *, int x, int y, wxRadioButton **, const wxString &);

    int CreateLabelGroup(wxWindow *, int x, int y, const wxString &);

private:

    void OnKeyDown(wxKeyEvent &);
};
