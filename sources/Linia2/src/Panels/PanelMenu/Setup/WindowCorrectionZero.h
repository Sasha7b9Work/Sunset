// 2025/7/27 13:41:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"


// Настройка - Коррекция смещения нуля


class WindowCorretionZero : public Dialog
{
public:

    static const int WIDTH = 350;
    static const int HEIGHT = 500;

    WindowCorretionZero();

private:

    wxRadioButton *rbChanC_MeasI = nullptr,
        *rbChanC_MeasU = nullptr,
        *rbChanB_MeasI = nullptr,
        *rbChanB_MeasU = nullptr,
        *rbChanB_SourceI = nullptr,
        *rbChanB_SourceU = nullptr,
        *rbChanS_MeasI = nullptr,
        *rbChanS_MeasU = nullptr,
        *rbChanS_SourceI = nullptr,
        *rbChanS_SourceU = nullptr;

    void OnEventButton(wxCommandEvent &);
};
