// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ButtonsCombo.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"
#include "Controls/StaticBox.h"
#include "Utils/Configurator.h"


DrawingButton::DrawingButton(wxWindow *parent, int id, const wxString &label, const wxPoint &position, const wxSize &size, const wxString &_name_file) :
    wxButton(parent, id, label, position, size),
    file_name(_name_file)
{
    wxButton::SetBackgroundStyle(wxBG_STYLE_PAINT); // Для избежания мерцания

    Bind(wxEVT_PAINT, &DrawingButton::OnPaint, this);

    wxButton::SetBackgroundColour(GetBackgroundColour().ChangeLightness(LIGHTNESS));
}


void DrawingButton::OnPaint(wxPaintEvent &)
{
    if (file_name[0])
    {

    }
}


class ButtonPopup : public wxPopupTransientWindow
{
public:
    ButtonPopup(wxWindow *parent) : wxPopupTransientWindow(parent)
    {
        wxPopupTransientWindow::Hide();

        wxArrayString &labels = GetCombo()->labels;

        // Основной контейнер с отступами по краям
        wxPanel *mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

        int num_cells = (int)labels.size() + GetCombo()->num_empties;                // Ячеек может быть больше, чем вариантов выбора - если нам нужно выравниивание по столбцам

        int num_rows = (int)(num_cells / GetCombo()->buttons_in_row);

        if (num_rows * GetCombo()->buttons_in_row < num_cells)
        {
            num_rows++;
        }

        int num_cols = num_cells / num_rows;

        if (num_rows * num_cols < num_cells)
        {
            num_cols++;
        }

        wxGridSizer *gridSizer = new wxGridSizer(num_rows, num_cols, 2, 2);

        // Добавляем рамку вокруг сетки кнопок
        StaticBoxSizer *boxSizer = new StaticBoxSizer(wxVERTICAL, mainPanel, GetCombo()->title);
        boxSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 0); // 10px отступ внутри рамки

        for (int i = 0; i < GetCombo()->num_empties; i++)
        {
            wxPanel *panel = new wxPanel(mainPanel);
            gridSizer->Add(panel, 0, wxEXPAND | wxALL, 2);
        }

        for (uint i = 0; i < labels.size(); ++i)
        {
            wxButton *btn = new wxButton(mainPanel, wxID_ANY, labels[i], wxDefaultPosition, wxDefaultSize, GetCombo()->left_align ? wxBU_LEFT : 0);
            btn->SetBackgroundColour(btn->GetBackgroundColour().ChangeLightness(LIGHTNESS));
            btn->SetBackgroundColour(btn->GetBackgroundColour().ChangeLightness(170));
            btn->SetMaxSize(wxSize(-1, 20));
            if (labels[i] != GetCombo()->tooltips[i])
            {
                btn->SetToolTip(GetCombo()->tooltips[i]);
            }
            btn->Bind(wxEVT_BUTTON, &ButtonPopup::OnButtonClick, this);
            gridSizer->Add(btn, 0, wxEXPAND | wxALL, 2);
        }

        // Основная панель
        mainPanel->SetSizer(boxSizer);

        wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
        // Внешние отступы 15px
        outerSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 3);
        SetSizer(outerSizer);

        wxPopupTransientWindow::Layout();

        wxPopupTransientWindow::Fit(); // Автоподбор размера

        GetParent()->Bind(wxEVT_KEY_DOWN, &ButtonPopup::OnKeyDown, this);

        wxPopupTransientWindow::Refresh();
        wxPopupTransientWindow::Update();

        wxPopupTransientWindow::SetBackgroundColour(GetBackgroundColour().ChangeLightness(50));

        // Отключаем изменение фона для всех детей
        for (auto child : GetChildren())
        {
            child->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
            child->SetBackgroundStyle(wxBG_STYLE_ERASE);
            child->Refresh(); // Обновляем внешний вид
        }

        wxPopupTransientWindow::SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY | wxWS_EX_PROCESS_UI_UPDATES);

        wxPopupTransientWindow::Show();
    }

private:

    ButtonsCombo *GetCombo()
    {
        return (ButtonsCombo *)GetParent();
    }

    void OnButtonClick(wxCommandEvent &event)
    {
        wxString label = ((ButtonBitmap *)event.GetEventObject())->GetLabel();

        for (size_t i = 0; i < GetCombo()->labels.size(); i++)
        {
            if (label == GetCombo()->labels[i])
            {
                ButtonsCombo *combo = (ButtonsCombo *)GetParent();

                combo->SetCurrentSelection((int)i);

                Dismiss();

                break;
            }
        }
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



ButtonsCombo::ButtonsCombo(wxWindow *parent, const wxString &_title, const wxPoint &pos, int width,
    const wxArrayString &_labels, const wxArrayString &_tooltips, int _buttons_in_row, const wxString &name, Type::E type) :
    DrawingButton(parent, wxID_ANY, _labels[0], pos, { width, TEXTCNTRL_HEIGHT + 3 }, (type == Type::Bitmap) ? _title : wxString("")),
    current_choice(0)
{
    colorBackground = DrawingButton::GetBackgroundColour();
    colorForeground = DrawingButton::GetForegroundColour();

    DrawingButton::SetName(parent->GetName() + "_" + name);

    Bind(wxEVT_BUTTON, &ButtonsCombo::OnButtonClicked, this);
    Bind(wxEVT_LEFT_DOWN, &ButtonsCombo::OnMouseDown, this);

    title = (type == Type::Text) ? _title : wxString();

    SetChoices(_labels, _tooltips);

    SetCurrentSelection(0);

    buttons_in_row = _buttons_in_row;
}


void ButtonsCombo::OnButtonClicked(wxCommandEvent &event)
{
    if (labels.size() > 1)
    {
        if (left_part_clicked)
        {
            int choice = current_choice + 1;

            if ((uint)choice >= labels.size())
            {
                choice = 0;
            }

            SetCurrentSelection(choice);
        }
        else
        {
            ButtonPopup *popup = new ButtonPopup(this);

            wxPoint pos = ClientToScreen(wxPoint(GetSize().x / 2, GetSize().y / 2));

            wxSize size = popup->GetSize();

            pos.x -= size.x / 2;
            pos.y -= size.y / 2;

            if (GF::IsBoardPCM())
            {
                if (pos.x + size.x >= MainWindow::WIDTH)
                {
                    int delta = pos.x + size.x - MainWindow::WIDTH;
                    pos.x -= delta + 8;
                }
                else if (pos.x < 5)
                {
                    pos.x = 5;
                }
            }
            else
            {
                int right_pop = pos.x + size.x;
                int right_win = TheMainWindow->GetPosition().x + TheMainWindow->GetSize().x; //-V807

                if (right_pop >= right_win)
                {
                    int delta = right_pop - right_win;
                    pos.x -= delta + 8;
                }
                else if (pos.x < TheMainWindow->GetPosition().x + 5)
                {
                    pos.x = TheMainWindow->GetPosition().x + 5;
                }
            }

            popup->Position(pos, wxSize(0, 0));
            popup->Popup();
            popup->Refresh();
            popup->Update();
            popup->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
        } //-V773
    }

    event.Skip();
}


void ButtonsCombo::OnMouseDown(wxMouseEvent &event)
{
    left_part_clicked = event.GetPosition().x < GetSize().x / 2;

    event.Skip();
}


void ButtonsCombo::SetCurrentSelection(int choice)
{
    DrawingButton::SetBackgroundColour(colorBackground);
    DrawingButton::SetForegroundColour(colorForeground);
    DrawingButton::Refresh();

    bool need_event = (choice != current_choice);

    current_choice = choice;

    wxString label;

    if (title[0])
    {
        label += title + " : ";
    }

    uint index = (uint)current_choice;

    SetMyToolTip((tooltips[index] != labels[index]) ?     // Предполагается, что подсказка устанавливается только в том случае, если она не совпадает с надписью
        (tooltips[index]) : wxString());

    SetExtendedLabel(label, labels[index]);

    if(need_event)
    {
        GF::SendCommandEvent(this, wxEVT_COMBOBOX, GetCurrentSelection());
    }
}


void ButtonsCombo::SetMyToolTip(const wxString &tool)
{
    myToolTip = tool;
    SetToolTip(myToolTip);
}


wxString ButtonsCombo::GetMyToolTip() const
{
    return myToolTip;
}


void ButtonsCombo::SetLastSelection()
{
    SetCurrentSelection((int)labels.GetCount() - 1);
}


int ButtonsCombo::GetCurrentSelection() const
{
    return current_choice;
}


wxString ButtonsCombo::GetCurrentString() const
{
    return labels[(uint)(current_choice)];
}


void ButtonsCombo::Pack()
{
    Config::WriteString(GetName(), GetCurrentString());
}


void ButtonsCombo::Unpack()
{
    wxString selection = Config::ReadString(GetName());

    SetCurrentString(selection);
}


void ButtonsCombo::SetChoices(const wxArrayString &choices, const wxArrayString &_tooltips)
{
    labels = choices;
    tooltips = _tooltips;

    if (insert_empty)
    {
        if (choices[0][0] == '2')
        {
            num_empties = 1;
        }
        else if (choices[0][0] == '4' || choices[0][0] == '5')
        {
            num_empties = 2;
        }
    }

    SetCurrentSelection(GetCurrentSelection() >= NumChoices() ? 0 : GetCurrentSelection());
}


const wxArrayString &ButtonsCombo::GetChoices() const
{
    return labels;
}


int ButtonsCombo::NumChoices() const
{
    return (int)labels.Count();
}


void ButtonsCombo::SetCurrentString(const wxString &choice)
{
    for (uint i = 0; i < labels.size(); i++)
    {
        if (labels[i] == choice)
        {
            SetCurrentSelection((int)i);
            break;
        }
    }
}


void ButtonsCombo::SetTitle(const wxString &_title)
{
    title = _title;

    SetCurrentSelection(GetCurrentSelection());
}


void ButtonsCombo::SetExtendedLabel(const wxString &start, const wxString &end)
{
    if (start.IsEmpty())
    {
        DrawingButton::SetLabel(end);
    }
    else
    {
        int num_spaces = 0;

        while (true)
        {
            SetExtendedLabel(start, num_spaces, end);

            wxSize size = GetTextExtent(DrawingButton::GetLabel());

            if (size.x >= GetClientSize().x - 12)
            {
                if (num_spaces > 0)
                {
                    num_spaces--;
                }

                break;
            }

            num_spaces++;
        }

        SetExtendedLabel(start, num_spaces, end);
    }
}


void ButtonsCombo::SetExtendedLabel(const wxString &start, int num_spaces, const wxString &end)
{
    wxString label{ start };
    label.Append(' ', (size_t)num_spaces);
    label.Append(end);
    SetLabel(label);
}


ButtonsComboRange::ButtonsComboRange(wxWindow *parent, const wxString &title, const wxPoint &pos, int width, const wxArrayString &labels,
    const wxArrayString &tooltips, const wxString &name) :
    ButtonsCombo(parent, title, pos, width, labels, tooltips, 3, name, ButtonsCombo::Type::Text)
{
    ButtonsCombo::insert_empty = true;

    ButtonsCombo::SetChoices(labels, tooltips);
}
