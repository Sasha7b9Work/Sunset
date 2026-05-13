// 2026/04/08 15:30:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Panel.h"


class ToggleButton;
class Notebook;
class wxBoxSizer;


// Это панель - со своей кнопкой выбора, с возможностью открепляться от PanelBoard
class PageNotebook : public Panel
{
    friend class Notebook;

public:

    PageNotebook(Notebook *, const wxString &);

    const wxString &GetPanelName() const;

private:

    wxString name;
};


class Notebook : public Panel
{
public:

    Notebook(wxWindow *);

    void AddPanel(PageNotebook *);

    wxPanel *GetCenterContainer() const
    {
        return centerContainer;
    }

    int GetCurrentPanelIndex() const;

    // Переключиться на панель по указателю
    void SetCurrentPanel(PageNotebook *);

private:

    wxBoxSizer *mainSizer = nullptr;        // Вертикальный главный sizer
    wxBoxSizer *topSizer = nullptr;         // Горизонтальный sizer для верхних кнопок

    wxPanel *centerContainer = nullptr;     // Контейнер для центральной области
    wxBoxSizer *centerSizer = nullptr;      // Sizer для центрального контейнера

    std::vector<ToggleButton *> buttons;    // Кнопки, соотвествующие панелям
    PageNotebook *currentPanel = nullptr;   // Текущая активная панель

    // Добавить кнопку в верхнюю панель
    void AddTopButton(PageNotebook *);

    void OnEventButtonToggle(wxCommandEvent &);

    // Переключиться на панель по индексу
    void SetCurrentPanelIndex(int index);

    int GetPanelIndex(PageNotebook *) const;
};
