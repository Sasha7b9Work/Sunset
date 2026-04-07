// 2026/3/16 21:44:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelCalc/WindowCursors.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Utils/SystemDepend.h"


WindowCursors *TheWindowCursors = nullptr;


WindowCursors::WindowCursors() : DraggedDialog(wxT("Курсоры"), {200, 300})
{
    int x = 10;
    int y = 22;
    int dy = 25;

    wxWindow *wnd = MainWidget();

    wxSize size = wnd->GetClientSize();

    const int d = 5;

    size.x -= 5 + 5;
    size.y = 100;

    wxStaticBox *boxAuto = new wxStaticBox(wnd, wxID_ANY, "Автоматические", { d, TopY() }, size);

    {
        chbTrackX = new ValueCheckBox(boxAuto, "Отслеживать по X", { x, SD::Y_SB(y) }, "track_x", false);

        y += dy;

        chbTrackY = new ValueCheckBox(boxAuto, "Отслеживать по Y", { x, SD::Y_SB(y) }, "track_y", false);

        y += dy;

        chbTrackMouse = new ValueCheckBox(boxAuto, "Указатель мыши", { x, SD::Y_SB(y) }, "track_mouse", false);
    }

    wxStaticBox *boxManual = new wxStaticBox(wnd, wxID_ANY, "Управляемые", { d, boxAuto->GetPosition().y + boxAuto->GetSize().y + d }, size);

    y = 22;

    {
        chbCursorsX = new ValueCheckBox(boxManual, "X", { x, SD::Y_SB(y) }, "cursors_x", false);

        y += dy;

        chbCursorsY = new ValueCheckBox(boxManual, "Y", { x, SD::Y_SB(y) }, "cursors_y", false);
    }
}
