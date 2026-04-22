// 2025/6/1 16:29:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Buttons.h"


// Это панель - со своей кнопкой выбора, с возможностью открепляться от PanelBoard
class Panel : public wxPanel
{
    friend class Notebook;

public:

    Panel(Notebook *, const wxString &);

    const wxString &GetPanelName() const;

protected:

    // Эта функция будет вызываться при наступлении события wxEVT_SIZE
    virtual void CallbackOnEventSize() = 0;

private:

    wxString name;
    wxSize prev_size{ -1, -1 };             // Здесь храним размер, обработанный в предыдущем событии wxEVT_SIZE

    void OnEventSize(wxSizeEvent &);
};
