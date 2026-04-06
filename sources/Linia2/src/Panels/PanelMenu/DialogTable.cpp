// 2025/7/12 17:38:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/DialogTable.h"
#include "Panels/PanelTable.h"


DialogTable *DialogTable::self = nullptr;


const wxString DialogTable::BTN_SAVE = wxT("Сохранить");
const wxString DialogTable::BTN_DELETE = wxT("Удалить");
const wxString DialogTable::BTN_EXPAND = wxT("Показать");
const wxString DialogTable::BTN_COLLAPSE = wxT("Скрыть");


DialogTable::DialogTable() :
    MenuDialog(wxT("Таблица"), 100, { },
        BTN_SAVE, []()
        {
        },
        BTN_DELETE, []()
        {
        },
        BTN_EXPAND, []()
        {
            PanelTable::self->Show();

            DialogTable::self->Close();
        },
        BTN_COLLAPSE, []()
        {
            PanelTable::self->Hide();

            DialogTable::self->Close();
        }
    )
{
    self = this;

    FindButton(BTN_EXPAND)->Enable(!PanelTable::self->IsShown());

    FindButton(BTN_COLLAPSE)->Enable(PanelTable::self->IsShown());
}
