// 2026/04/03 11:49:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/StaticBoxCustom.h"


StaticBoxCustom::StaticBoxCustom(wxWindow *parent, const wxString &title, const wxPoint &position, const wxSize &size, std::vector<std::unique_ptr<DescriptionControl>> &controls) :
    StaticBox(parent, title, position, size)
{
    for (auto &desc : controls)
    {
        desc->Create(this);
    }
}
