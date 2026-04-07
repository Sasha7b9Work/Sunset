// 2025/09/01 23:00:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Error
{
    enum E
    {
        _1 = 1,
        _2,
        _3,
        _4,
        Count
    };
};


class PanelErrors : public wxPanel
{
    friend struct PanelDisplay;

public:

    void ReInit();

    void AppendError(Error::E, const wxString &);
    void RemoveError(Error::E, const wxString &);

private:

    PanelErrors(wxWindow *);

    wxButton *btnCollapse = nullptr;

    wxTextCtrl *text_ctrl = nullptr;

    bool collapse = false;

    wxTimer timer;

    struct StructError
    {
        Error::E err;
        wxString message;
    };

    std::vector<StructError> errors;

    void OnEventButton(wxCommandEvent &);
    void OnEventTimer(wxTimerEvent &);

    void SetColors(bool inverse);

    void BuildCanvas();
};
