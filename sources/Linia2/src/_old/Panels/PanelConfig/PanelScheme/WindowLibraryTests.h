// 2025/7/24 22:47:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"


class WindowLibraryTests : public Dialog
{
public:

    static const int WIDTH = 700;
    static const int HEIGHT = 255;

    WindowLibraryTests();

private:

    wxGrid *grid = nullptr;

    void SetTitlesColumn();

    void SetAutoSizeColumns();

    void Fill();

    void AppendLine(const wxString &, const wxString &, const wxString &);
};
