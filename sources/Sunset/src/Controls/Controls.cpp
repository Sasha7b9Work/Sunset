// 2026/04/03 12:06:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Controls.h"


void DescRadioButton::Create(wxWindow *parent)
{
    *pointer = new wxRadioButton(parent, wxID_ANY, name, wxDefaultPosition, wxDefaultSize, flags);
}
