// 2026/04/29 15:55:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Panel.h"


class ::Category;


class PanelLibrary : public Panel
{
public:

    PanelLibrary(wxWindow *, PanelLibrary *&);

    void BuildListTests(std::vector<const ::Category *> &categories);
};
