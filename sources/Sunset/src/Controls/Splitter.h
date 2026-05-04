// 2026/04/22 09:26:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/splitter.h>
#pragma warning(pop)


class Splitter : public wxSplitterWindow
{
public:
    Splitter(wxWindow *, int flags);
};
