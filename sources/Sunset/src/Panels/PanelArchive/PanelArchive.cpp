// 2026/04/08 15:06:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelArchive/PanelArchive.h"


PanelArchive *ThePanelArchive = nullptr;


PanelArchive::PanelArchive(PanelBoard *board) : Panel(board, L("Архив"))
{
    ThePanelArchive = this;
}


void PanelArchive::CallbackOnEventSize()
{

}
