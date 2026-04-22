// 2026/04/08 15:30:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ToggleButton;


// Это панель - со своей кнопкой выбора, с возможностью открепляться от PanelBoard
class PageNotebook : public wxPanel
{
    friend class Notebook;

public:

    PageNotebook(Notebook *, const wxString &);

    const wxString &GetPanelName() const;

protected:

    // Эта функция будет вызываться при наступлении события wxEVT_SIZE
    virtual void CallbackOnEventSize() = 0;

private:

    wxString name;
    wxSize prev_size{ -1, -1 };             // Здесь храним размер, обработанный в предыдущем событии wxEVT_SIZE

    void OnEventSize(wxSizeEvent &);
};


class Notebook : public wxPanel
{
public:

    Notebook(wxWindow *);

    void AddPanel(PageNotebook *);

    // Переключиться на панель по индексу
    void SetCurrentPanelIndex(int index);

    wxPanel *GetCenterContainer() const
    {
        return centerContainer;
    }

    int GetCurrentPanelIndex() const;

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

    // Переключиться на панель по указателю
    void SetCurrentPanel(PageNotebook *);

    int GetPanelIndex(PageNotebook *) const;
};
