// 2026/04/08 15:30:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panels.h"


// Это место, где будут храниться панели
struct PanelBoard : public wxPanel
{
    PanelBoard(wxWindow *);

    void AddPanel(Panel *);

    // Переключиться на панель по индексу
    void ShowPanel(int index);

    // Переключиться на панель по указателю
    void ShowPanel(Panel *panel);

private:

    wxBoxSizer *mainSizer = nullptr;        // Вертикальный главный sizer
    wxBoxSizer *topSizer = nullptr;         // Горизонтальный sizer для верхних кнопок

    wxPanel *centerContainer = nullptr;     // Контейнер для центральной области
    wxBoxSizer *centerSizer = nullptr;      // Sizer для центрального контейнера

    std::vector<ToggleButton *> buttons;    // Кнопки, соотвествующие панелям
    Panel *currentPanel = nullptr;          // Текущая активная панель

    // Добавить кнопку в верхнюю панель
    void AddTopButton(Panel *);

    void OnEventButtonToggle(wxCommandEvent &);
};
