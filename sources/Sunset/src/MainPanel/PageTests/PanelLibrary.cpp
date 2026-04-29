// 2026/04/29 15:56:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainPanel/PageTests/PanelLibrary.h"


PanelLibrary *ThePanelLibrary = nullptr;


PanelLibrary::PanelLibrary(wxWindow *parent, PanelLibrary *&global) : Panel(parent)
{
    global = this;
}
