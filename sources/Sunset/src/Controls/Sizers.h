// 2026/04/23 11:53:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/sizer.h>
#pragma warning(pop)


// Сюда добавляются виджеты сверху вниз с определённым расстоянием между ними
class SizerVert : public wxBoxSizer
{
public:

    SizerVert();

    void AddSizer(wxBoxSizer *);

    // Добавить сайзер, в который поместить данный виджет
    void AddWidgetsInSizer(wxWindow *, wxWindow * = nullptr);

    void SetSpacer(int _spacer)
    {
        spacer = _spacer;
    }

private:

    int spacer = 10;
};


class SizerHor : public wxBoxSizer
{
public:

    SizerHor();

    void AddWidget(wxWindow *);

    void Finish();

private:

    int spacer = 10;
};
