// 2026/04/08 15:15:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelSettings/PanelSettings.h"


PanelSettings *ThePanelSettings = nullptr;


PanelSettings::PanelSettings(PanelBoard *board) : Panel(board, L("Настройки"))
{
    ThePanelSettings = this;
}


void PanelSettings::CallbackOnEventSize()
{

}

