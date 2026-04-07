// 2025/6/1 16:29:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Panel;


// Это место, где будут храниться панели
struct PanelBoard : public wxPanel
{
    PanelBoard(wxWindow *);

    void AddPanel(Panel *);

    // Переключиться на панель по индексу
    void ShowPanel(int index);

    // Переключиться на панель по указателю
    void ShowPanel(wxPanel *panel);

private:

    wxBoxSizer *m_mainSizer = nullptr;      // Вертикальный главный sizer
    wxBoxSizer *m_topSizer = nullptr;       // Горизонтальный sizer для верхних кнопок
    wxBoxSizer *m_bottomSizer = nullptr;    // Горизонтальный sizer для нижних кнопок

    wxPanel *m_centerContainer = nullptr;   // Контейнер для центральной области
    wxBoxSizer *m_centerSizer = nullptr;    // Sizer для центрального контейнера

    std::vector<wxPanel *> m_contentPanels; // Все добавленные панели
    std::vector<wxString> m_panelNames;     // Имена панелей (для отладки)
    wxPanel *m_currentPanel = nullptr;      // Текущая активная панель

    // Добавить кнопку в верхнюю панель
    void AddTopButton(const wxString &label, wxObject *eventUserData = nullptr);
    // Добавить кнопку в нижнюю панель
    void AddBottomButton(const wxString &label, wxObject *eventUserData = nullptr);

    void OnTopButtonClicked(wxCommandEvent &);

    void OnBottomButtonClicked(wxCommandEvent &);

    wxDECLARE_EVENT_TABLE();
};


// Это панель - со своей кнопкой выбора, с возможностью открепляться от PanelBoard
struct Panel : public wxPanel
{
    Panel(wxWindow *);
};
