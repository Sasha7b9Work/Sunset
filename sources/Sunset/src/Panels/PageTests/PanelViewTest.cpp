// 2026/04/29 16:03:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PageTests/PanelViewTest.h"


PanelViewTest *ThePanelViewTest = nullptr;


PanelViewTest::PanelViewTest(wxWindow *parent, PanelViewTest *&global) : Panel(parent)
{
    global = this;
}
