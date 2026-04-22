// 2025/09/01 22:59:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainPanel/PanelMeasures/PanelErrors.h"
#include "MainWindow.h"
#include "MainPanel/PanelMeasures/PanelMeasures.h"
#include "Utils/SystemDepend.h"
#pragma warning(push, 0)
#include <wx/sizer.h>
#pragma warning(pop)


PanelErrors *ThePanelErrors = nullptr;


PanelErrors::PanelErrors(wxWindow *parent) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { MainWindow::WIDTH_DRAW, 100 })
{
    ThePanelErrors = this;

    //    wxPanel *panel = new wxPanel(this);

    text_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, { 0, 0 }, { MainWindow::WIDTH_DRAW, 90 },
        wxTE_MULTILINE | wxTE_READONLY | wxNO_BORDER | wxVSCROLL | wxTE_RICH);
    text_ctrl->SetForegroundColour(*wxWHITE);
    text_ctrl->SetBackgroundColour(*wxRED);

    text_ctrl->SetDoubleBuffered(true);

    wxSize size_button{ 90, 22 };
    btnCollapse = new wxButton(this, wxID_ANY, "Свернуть", wxDefaultPosition, size_button);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(text_ctrl, 1, wxEXPAND | wxALL, 0);
    sizer->Add(btnCollapse, 0, wxTOP, 0);

    SetSizer(sizer);

    Layout();

    ReInit();

    Bind(wxEVT_BUTTON, &PanelErrors::OnEventButton, this);
    Bind(wxEVT_TIMER, &PanelErrors::OnEventTimer, this, timer.GetId());

    timer.SetOwner(this, timer.GetId());

    AppendError(Error::_1, "Мало памяти");
    AppendError(Error::_2, "Много памяти");

//    for (int i = 0; i < 10; i++)
//    {
//        AppendError(Error::_1, wxString::Format("Ошибка %d", i));
//    }

    RemoveError(Error::_1, "Мало памяти");
    RemoveError(Error::_2, "Много памяти");
}


void PanelErrors::ReInit()
{
    wxSize size = GetSize();

    size.x = ThePanelMeasures->GetFullSize().x;

    SetSize(size);                                                  // Корректируем размер под размер дисплея
}


void PanelErrors::OnEventButton(wxCommandEvent &event)
{
    collapse = !collapse;

    SetColors(false);

    if (collapse)
    {
        SetSize({ GetSize().x, 23 });
        btnCollapse->SetLabel(wxT("Развернуть"));
        timer.Start(500);
    }
    else
    {
        SetSize({ GetSize().x, 100 });
        btnCollapse->SetLabel(wxT("Свернуть"));
        timer.Stop();
    }

    event.Skip();
}


void PanelErrors::OnEventTimer(wxTimerEvent &)
{
    SetColors(text_ctrl->GetBackgroundColour() == *wxRED);
}


void PanelErrors::SetColors(bool inverse)
{
    text_ctrl->SetBackgroundColour(inverse ? *wxWHITE : *wxRED);

    text_ctrl->SetForegroundColour(inverse ? *wxBLACK : *wxWHITE);

    GetParent()->Refresh();
}


void PanelErrors::AppendError(Error::E err, const wxString &message)
{
    for (auto &elem : errors)
    {
        if (elem.err == err &&
            elem.message == message)
        {
            return;                     // Если такое сообщение уже есть - выходим. Добавлять такое же не будем
        }
    }

    errors.emplace_back(StructError{ err, message });

    BuildCanvas();

    Show();
}


void PanelErrors::RemoveError(Error::E err, const wxString &message)
{
    for (auto it = errors.begin(); it != errors.end(); ++it)
    {
        if (it->err == err &&
            it->message == message)
        {
            errors.erase(it);

            BuildCanvas();

            if (errors.empty())
            {
                Hide();
            }

            return;
        }
    }
}


void PanelErrors::BuildCanvas()
{
    wxString label;

    for (const auto &elem : errors)
    {
        if (!label.empty())
        {
            label += "\r\n";
        }
        label += wxString::Format("ERROR %d - %s", elem.err, elem.message);
    }

    text_ctrl->SetValue(label);
}
