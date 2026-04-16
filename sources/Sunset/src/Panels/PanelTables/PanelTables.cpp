// 2026/04/08 15:17:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelTables/PanelTables.h"


PanelTables *ThePanelTables = nullptr;


PanelTables::PanelTables(PanelBoard *board) : Panel(board, L("Таблицы"))
{
    ThePanelTables = this;
}


void PanelTables::CallbackOnEventSize()
{

}
