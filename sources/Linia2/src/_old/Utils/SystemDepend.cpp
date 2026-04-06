// 2024/11/02 18:44:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/SystemDepend.h"


wxSize SD::D::ChooseSound::SizeSpin()
{
#ifdef WIN32
    return { 40, TEXTCNTRL_HEIGHT };
#else
    return { 116, TEXTCNTRL_HEIGHT + 13 };       // Высоту меньше некоторого значения (на всех системах разного) нельзя
#endif
}


wxPoint SD::D::ChooseSound::PosSpin(int x, int y)
{
#ifdef WIN32
    return { x + 200, y };
#else
    return { x + 220, y };
#endif
}


wxSize SD::D::ChooseSound::SizeCombo()
{
#ifdef WIN32
    return { 50, TEXTCNTRL_HEIGHT };
#else
    return { 80, TEXTCNTRL_HEIGHT + 13 };
#endif
}


wxPoint SD::D::ChooseSound::PosCombo(int x, int y)
{
#ifdef WIN32
    return { x + 100, y };
#else
    return { x + 120, y };
#endif
}


int SD::D::ChooseSound::d()
{
#ifdef WIN32
    return 30;
#else
    return 40;
#endif
}


int SD::D::ChooseSound::y()
{
#ifdef WIN32
    return 130;
#else
    return 160;
#endif
}


wxSize SD::D::ChooseSound::Size()
{
#ifdef WIN32
    return { 300, 210 };
#else
    return { 450, 250 };
#endif
}


int SD::D::CommonPanel::Delta()
{
#ifdef WIN32
    return 10;
#else
    return -20;
#endif
}


wxSize SD::D::ColorDialog::SizeSlider()
{
#ifdef WIN32
    return { 250, 20 };
#else
    return { 250, 40 };
#endif
}


int SD::D::ColorDialog::Spacer()
{
#ifdef WIN32
    return 20;
#else
    return 10;
#endif
}


wxSize SD::D::ColorDialog::Size()
{
#ifdef WIN32
    return { 500, 300 };
#else
    return { 500, 450 };
#endif
}


int SD::Y_SB(int y)
{
#ifdef WIN32
    return y;
#else
    return y - 20;
#endif
}


wxPoint SD::XY0()
{
#ifdef WIN32
    return { 5, 20 };
#else
    return { 5 ,5 };
#endif
}


int SD::DSBY()
{
    return 5;
}
