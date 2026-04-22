// 2026/04/08 15:15:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainPanel/PanelSettings/PanelSettings.h"


PanelSettings *ThePanelSettings = nullptr;


PanelSettings::PanelSettings(Notebook *board, PanelSettings *&self) :
    PageNotebook(board, L("Настройки"))
{
    self = this;
}


void PanelSettings::CallbackOnEventSize()
{

}

