// 2026/3/16 21:09:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Value.h"
#pragma warning(push, 0)
#include <wx/frame.h>
#pragma warning(pop)


// Окно, которое можно передвигать мышью
class DraggedWindow : public wxFrame
{
public:

    DraggedWindow(const wxString &, const wxSize &);

    virtual bool Show(bool = true) override;

    void SetSize(const wxSize &);

protected:

    wxPanel *main_panel = nullptr;
    ValuePoint *position = nullptr;

    virtual ~DraggedWindow();

    int TopY() const
    {
        return titleHeight + 10;
    }

    wxWindow *MainWidget()
    {
        return main_panel;
    }

private:

    wxPanel *titleBar = nullptr;
    bool     dragging = false;
    wxPoint  dragStart;
    wxString title;
    wxRect   closeButtonRect;               // Область кнопки закрытия
    bool     mouseInCloseButton = false;    // Для ховер-эффекта
    const int titleHeight = 20;

    void CreateTitleBar();

    void SetPosition();

    void SetupDragging(wxWindow *);

    void OnEventMouseLeftDown(wxMouseEvent &);
    void OnEventMouseLeftUp(wxMouseEvent &);
    void OnEventMouseMotion(wxMouseEvent &);
    void OnEventMouseLeave(wxMouseEvent &);
    void OnEventPaint(wxPaintEvent &);
    void OnEventClose(wxCloseEvent &);
};


// В отличие от DraggedWindow, является модальным, т.е. все окна кроме этого неактивны
class DraggedDialog : public DraggedWindow
{
public:

    DraggedDialog(const wxString &, const wxSize &);
    virtual ~DraggedDialog();

    int ShowModal();

protected:

    void CloseModal();

private:

    virtual bool Show(bool = true) override;

    bool m_modalActive = false;
    int m_modalResult = wxID_CANCEL;
    wxWindow *m_parent = nullptr;

    void ShowWithXFCEFix();

    void OnEventClose(wxCloseEvent &);

    void OnOK(wxCommandEvent &);

    void OnCancel(wxCommandEvent &);
};
