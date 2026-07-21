// 2026/04/08 15:06:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelArchive/PanelArchive.h"


PanelArchive *PanelArchive::self = nullptr;


PanelArchive::PanelArchive(Notebook *board) : PageNotebook(board, L("Архив"))
{
    self = this;
}
