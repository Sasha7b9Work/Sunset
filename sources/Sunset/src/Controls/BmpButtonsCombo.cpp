// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/BmpButtonsCombo.h"
#include "Utils/GlobalFunctions.h"
#include "Controls/StaticBox.h"
#include "Utils/Configurator.h"
#include "Settings/Settings.h"
#pragma warning(push, 0)
#include <wx/settings.h>
#pragma warning(pop)


class BmpButtonPopup : public wxPopupTransientWindow
{
public:
    BmpButtonPopup(wxWindow *parent, const wxString &title, const wxArrayString &files, const wxArrayString &tooltips, int buttons_in_row) :
        wxPopupTransientWindow(parent)
    {
        wxPopupTransientWindow::Hide();

        // Основной контейнер с отступами по краям
        wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
        wxPanel *mainPanel = new wxPanel(this, wxID_ANY);

        int num_rows = (int)(files.size() / buttons_in_row);

        if (num_rows * buttons_in_row < (int)files.size())
        {
            num_rows++;
        }

        int num_cols = (int)(files.size() / num_rows);

        if (num_rows * num_cols < (int)files.size())
        {
            num_cols++;
        }

        wxGridSizer *gridSizer = new wxGridSizer(num_rows, num_cols, 2, 2); // 5px промежутки

        // Добавляем рамку вокруг сетки кнопок
        StaticBoxSizer *boxSizer = new StaticBoxSizer(wxVERTICAL, mainPanel, title);
        boxSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 0); // 10px отступ внутри рамки

        for (uint i = 0; i < files.size(); ++i)
        {
            ButtonBitmap *btn = new ButtonBitmap(mainPanel, wxDefaultSize, files[i], "");
            btn->SetBackgroundColour(btn->GetBackgroundColour().ChangeLightness(170));
            btn->SetLabel(wxString::Format("%d", i));
            btn->SetToolTip(tooltips[i]);
            btn->Bind(wxEVT_BUTTON, &BmpButtonPopup::OnButtonClick, this);
            gridSizer->Add(btn, 0, wxEXPAND | wxALL, 2); // 2px отступы у кнопок
        }

        // Основная панель
        mainPanel->SetSizer(boxSizer);

        // Внешние отступы 15px
        outerSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 3);
        SetSizer(outerSizer);

        wxPopupTransientWindow::Fit();

        GetParent()->Bind(wxEVT_KEY_DOWN, &BmpButtonPopup::OnKeyDown, this);

        wxPopupTransientWindow::SetBackgroundColour(GetBackgroundColour().ChangeLightness(70));

        // Отключаем изменение фона для всех детей
        for (auto child : GetChildren())
        {
            child->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
            child->SetBackgroundStyle(wxBG_STYLE_ERASE);
            child->Refresh(); // Обновляем внешний вид
        }

        wxPopupTransientWindow::SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY | wxWS_EX_PROCESS_UI_UPDATES);

        wxPopupTransientWindow::Show();

        SetupDragging(mainPanel);
        SetupDragging(this);
    }

private:

    bool     dragging = false;
    wxPoint  dragStart;

    void SetupDragging(wxWindow *window)
    {
        window->Bind(wxEVT_LEFT_DOWN, &BmpButtonPopup::OnDragStart, this);
        window->Bind(wxEVT_LEFT_UP, &BmpButtonPopup::OnEventMouseLeftUp, this);
        window->Bind(wxEVT_MOTION, &BmpButtonPopup::OnDragMotion, this);
    }

    void OnDragStart(wxMouseEvent &event)
    {
        if (!dragging)
        {
            wxWindow *source = (wxWindow *)event.GetEventObject();
            if (source && !source->HasCapture())
            {
                source->CaptureMouse();
            }

            dragging = true;
            dragStart = wxGetMousePosition();

        }

        event.Skip();
    }


    void OnEventMouseLeftUp(wxMouseEvent &event)
    {
        if (dragging)
        {
            wxWindow *source = (wxWindow *)event.GetEventObject();
            if (source && source->HasCapture())
            {
                source->ReleaseMouse();
            }

            dragging = false;
        }

        event.Skip();
    }


    void OnDragMotion(wxMouseEvent &event)
    {
        wxWindow *source = (wxWindow *)event.GetEventObject();

        if (dragging && event.Dragging() && source && source->HasCapture())
        {
            wxPoint currentPos = wxGetMousePosition();
            wxPoint delta = currentPos - dragStart;
            wxPoint newPos = GetPosition() + delta;
            Move(newPos);
            dragStart = currentPos;
        }

        event.Skip();
    }

    void OnButtonClick(wxCommandEvent &event)
    {
        wxString label = ((ButtonBitmap *)event.GetEventObject())->GetLabel();

        BmpButtonsCombo *combo = (BmpButtonsCombo *)GetParent();

        int choice = -1;

        label.ToInt(&choice);

        combo->SetCurrentChoice(choice);

        Dismiss();
    }

    void OnKeyDown(wxKeyEvent &event)
    {
        if (event.GetKeyCode() != WXK_SPACE)
        {
            Dismiss();
        }

        event.Skip();
    }
};


BmpButtonsCombo::BmpButtonsCombo(wxWindow *parent, const wxString &_title,
    const wxSize &size, const wxArrayString &_files, const wxArrayString &_tooltips, int num_file, int _buttons_in_row, const wxString &_name) :
    ButtonBitmap(parent, size, _files[(size_t)num_file], _name),
    current_choice(num_file),
    title(_title),
    files(_files),
    m_tooltips(_tooltips)
{
    Bind(wxEVT_BUTTON, &BmpButtonsCombo::OnButtonClicked, this);

    ButtonBitmap::SetToolTip(_tooltips[(size_t)num_file]);

    buttons_in_row = _buttons_in_row;
}


void BmpButtonsCombo::OnButtonClicked(wxCommandEvent &)
{
    BmpButtonPopup *popup = new BmpButtonPopup(this, title, files, m_tooltips, buttons_in_row);

    wxPoint pos = ClientToScreen(wxPoint(GetSize().x / 2, GetSize().y / 2));
    pos.x -= popup->GetSize().x / 2;
    pos.y -= popup->GetSize().y / 2;

    popup->Position(pos, wxSize(0, 0));
    popup->Popup();
    popup->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
} //-V773


void BmpButtonsCombo::SetCurrentChoice(int choice)
{
    current_choice = choice;

    uint index = (uint)current_choice;

    SetFileBitmap(files[index]);

    SetToolTip(m_tooltips[index]);

    GF::SendCommandEvent(this, wxEVT_COMBOBOX, GetCurrentChoice());
}


int BmpButtonsCombo::GetCurrentChoice() const
{
    return current_choice;
}


void BmpButtonsCombo::Pack()
{
    Config::WriteInt(GetName(), GetCurrentChoice());
}


void BmpButtonsCombo::Unpack()
{
    int choice = Config::ReadInt(GetName(), 0);

    SetCurrentChoice(choice);
}
