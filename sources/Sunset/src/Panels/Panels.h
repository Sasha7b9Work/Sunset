// 2025/6/1 16:29:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Это место, где будут храниться панели
struct PanelBoard : public wxPanel
{
    PanelBoard(wxWindow *);
};


// Это панель - со своей кнопкой выбора, с возможностью открепляться от PanelBoard
struct Panel : public wxPanel
{
    Panel(wxWindow *, int x, int y, int w, int h);
};
