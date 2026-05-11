// 2026/04/08 15:10:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelReports/PanelReports.h"


PanelReports *ThePanelReports = nullptr;


PanelReports::PanelReports(Notebook *board, PanelReports *&self) :
    PageNotebook(board, L("Отчёты"))
{
    self = this;
}
