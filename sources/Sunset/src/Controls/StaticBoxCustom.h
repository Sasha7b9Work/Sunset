// 2026/04/03 11:45:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/StaticBox.h"
#include "Controls/Controls.h"


class StaticBoxCustom : public StaticBox
{
public:

    StaticBoxCustom(wxWindow *, const wxString &, const wxSize &, std::vector<std::unique_ptr<DescriptionControl>> &);
};
