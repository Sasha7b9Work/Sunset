// 2025/10/07 12:29:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageSource50V.h"


/*
    Регистр управления      8 разрядов
*/


PageSource50V *PageSource50V::self = nullptr;


PageSource50V::PageSource50V(wxNotebook *parent) :
    PageChip(parent, "Источник 50 В")
{
    self = this;

    RegAD5443 *regDAC7 = new RegAD5443(this, dacs[6], "");

    regDAC7->Disable();

    AppendRegister(regDAC7);
}
