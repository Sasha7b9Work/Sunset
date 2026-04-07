// 2025/7/12 18:36:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/DialogReport.h"


DialogReport *DialogReport::self = nullptr;


const wxString DialogReport::BTN_NEW = wxT("Новый");
const wxString DialogReport::BTN_SAVE = wxT("Сохранить");
const wxString DialogReport::BTN_DELETE = wxT("Удалить");
const wxString DialogReport::BTN_EXPAND = wxT("Развернуть");
const wxString DialogReport::BTN_COLLAPSE = wxT("Свернуть");
const wxString DialogReport::BTN_CLOSE = wxT("Закрыть");


DialogReport::DialogReport() :
    MenuDialog(wxT("Отчёт"), 100, { },
        BTN_NEW, []()
        {
        },
        BTN_SAVE, []()
        {
        },
        BTN_DELETE, []()
        {
        },
        BTN_EXPAND, []()
        {
        },
        BTN_COLLAPSE, []()
        {
        },
        BTN_CLOSE, []()
        {
        }
    )
{
    self = this;
}
