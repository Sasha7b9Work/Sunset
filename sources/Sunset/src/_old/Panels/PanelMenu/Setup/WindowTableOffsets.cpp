// 2025/7/27 21:36:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/Setup/WindowTableOffsets.h"
#include "Utils/SystemDepend.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Math.h"
#include "IPPP/Tests/DSettings.h"


WindowTableOffsets::WindowTableOffsets() :
    Dialog(wxT("Таблица смещений и коэффициентов"), wxDefaultPosition, { WIDTH, HEIGHT })
{
    CreateFields();

    int x = 500;
    int y = 20;

    wxArrayString choices;
    choices.Add(wxT("смещений"));
    choices.Add(wxT("коэффициентов"));

    comboType = new ButtonsCombo(this, wxT("Таблица"), { x, SD::Y_SB(y - 3) }, 130, choices, choices, 1, "comboType");

    y = CreateLabelGroup(this, x, y + 20, wxT("Канал С"));
    y = CreateRadioButton(this, x, y, &rbChanC_MeasI, wxT("Измеритель I"));
    y = CreateRadioButton(this, x, y, &rbChanC_MeasU, wxT("Измеритель U"));
    rbChanC_MeasI->SetFocus();

    y = CreateLabelGroup(this, x, y, wxT("Канал B"));
    y = CreateRadioButton(this, x, y, &rbChanB_MeasI, wxT("Измеритель I"));
    y = CreateRadioButton(this, x, y, &rbChanB_MeasU, wxT("Измеритель U"));
    y = CreateRadioButton(this, x, y, &rbChanB_SourceI, wxT("Источник I"));
    y = CreateRadioButton(this, x, y, &rbChanB_SourceU, wxT("Источник U"));
    y = CreateRadioButton(this, x, y, &rbChanB_LimitI, wxT("Ограничение I"));
    y = CreateRadioButton(this, x, y, &rbChanB_LimitU, wxT("Ограничение U"));

    y = CreateLabelGroup(this, x, y, wxT("Канал S"));
    y = CreateRadioButton(this, x, y, &rbChanS_MeasI, wxT("Измеритель I"));
    y = CreateRadioButton(this, x, y, &rbChanS_MeasU, wxT("Измеритель U"));
    y = CreateRadioButton(this, x, y, &rbChanS_SourceI, wxT("Источник I"));
    y = CreateRadioButton(this, x, y, &rbChanS_SourceU, wxT("Источник U"));
    y = CreateRadioButton(this, x, y, &rbChanS_LimitI, wxT("Ограничение I"));
    y = CreateRadioButton(this, x, y, &rbChanS_LimitU, wxT("Ограничение U"));

    new wxButton(this, wxID_ANY, wxT("Применить"), { 50, HEIGHT - 100 }, { BUTTON_WIDTH, BUTTON_HEIGHT });
    new wxButton(this, wxID_ANY, wxT("Отменить"), { 250, HEIGHT - 100 }, { BUTTON_WIDTH, BUTTON_HEIGHT });

    Bind(wxEVT_RADIOBUTTON, &WindowTableOffsets::OnEventRadioButton, this);
    Bind(wxEVT_BUTTON, &WindowTableOffsets::OnEventButton, this);
    Bind(wxEVT_COMBOBOX, &WindowTableOffsets::OnEventCombobox, this);

    {
        // Включаем соотвествующие поля

        GF::SendCommandEvent(comboType, wxEVT_COMBOBOX, comboType->GetCurrentSelection());
    }
}


void WindowTableOffsets::CreateFields()
{
    wxPanel *panel = new wxPanel(this, wxID_ANY, { 10, 20 }, { 450, 460 });

    CreateFields_U(panel);
    CreateFields_I(panel);
}


void WindowTableOffsets::CreateFields_U(wxPanel *panel)
{
    const int d = 10;

    wxSize size1{ 90, TEXTCNTRL_HEIGHT };
    wxSize size2{ 130, TEXTCNTRL_HEIGHT };

    int dh = 1;

    wxSize size{ (size1.x + size2.x) * 2 + d, (size1.y + dh) * ((int)(RangeU::Count + 1) / 2 + 1) };

    Dialog::SetSizeHints(size);

    new wxStaticText(panel, wxID_ANY, "Диапазон", { 0, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Значение", { size1.x, 0 }, size2, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Диапазон", { size1.x + size2.x + d, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Значение", { d + (size1.x * 2) + size2.x, 0 }, size2, wxALIGN_CENTER);

    const int num_rows = (int)(RangeU::Count + 1) / 2;

    const int num_cols = 2;

    RangeU range = RangeU((RangeU::E)0);

    for (int col = 0; col < num_cols; col++)
    {
        for (int row = 0; row < num_rows; row++)
        {
            if (range.value < RangeU::Count)
            {
                int x = col * (size1.x + size2.x + d);
                int y = size1.y + row * (size1.y + dh);

                Field field;

                field.name = new wxStaticText(panel, wxID_ANY, "", {x, y}, size1, wxALIGN_CENTER);

                x += size1.x;

                field.value = new wxTextCtrl(panel, wxID_ANY, "", {x, y}, size2);

                field.range = range.value;

                fields_U.push_back(field);

                ++range;
            }
        }
    }
}


void WindowTableOffsets::CreateFields_I(wxPanel *panel)
{
    const int d = 10;

    wxSize size1{ 90, TEXTCNTRL_HEIGHT };
    wxSize size2{ 130, TEXTCNTRL_HEIGHT };

    int dh = 1;

    wxSize size{ (size1.x + size2.x) * 2 + d, (size1.y + dh) * ((int)(RangeI::Count + 1) / 2 + 1) };

    Dialog::SetSizeHints(size);

    new wxStaticText(panel, wxID_ANY, "Диапазон", { 0, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Значение", { size1.x, 0 }, size2, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Диапазон", { size1.x + size2.x + d, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Значение", { d + (size1.x * 2) + size2.x, 0 }, size2, wxALIGN_CENTER);

    const int num_rows = (int)(RangeI::Count + 1) / 2;

    const int num_cols = 2;

    RangeI range = RangeI((RangeI::E)0);

    for (int col = 0; col < num_cols; col++)
    {
        for (int row = 0; row < num_rows; row++)
        {
            if (range.value < RangeI::Count)
            {
                int x = col * (size1.x + size2.x + d);
                int y = size1.y + row * (size1.y + dh);

                Field field;

                field.name = new wxStaticText(panel, wxID_ANY, "", {x, y}, size1, wxALIGN_CENTER);

                x += size1.x;

                field.value = new wxTextCtrl(panel, wxID_ANY, "", { x, y }, size2);

                field.range = range.value;

                fields_I.push_back(field);

                ++range;
            }
        }
    }
}


void WindowTableOffsets::OnEventButton(wxCommandEvent &)
{

}


void WindowTableOffsets::OnEventRadioButton(wxCommandEvent &)
{
    FillFields();
}


void WindowTableOffsets::OnEventCombobox(wxCommandEvent &)
{
    FillFields();
}


void WindowTableOffsets::FillFields()
{
    bool show_I = rbChanC_MeasI->GetValue() ||
        rbChanB_MeasI->GetValue() ||
        rbChanB_SourceI->GetValue() ||
        rbChanB_LimitI->GetValue() ||
        rbChanS_MeasI->GetValue() ||
        rbChanS_SourceI->GetValue() ||
        rbChanS_LimitI->GetValue();

    int index = comboType->GetCurrentSelection();

    if (index == 0)
    {
        FillOffsets(CurrentMeasure(), show_I);
    }
    else if (index == 1)
    {
        FillK(CurrentMeasure(), show_I);
    }
    else
    {
        LOG_ERROR("Bad index");
    }

    if (show_I)
    {
        ShowFieldsI(true);
        ShowFieldsU(false);
    }
    else
    {
        ShowFieldsU(true);
        ShowFieldsI(false);
    }
}


void WindowTableOffsets::FillOffsets(TypeValue::E type, bool show_I)
{
    if (show_I)
    {
        for (uint i = 0; i < RangeI::Count; i++)
        {
            const CalK &cal = DSet::Get(type, (RangeI::E)i);

            fields_I[i].value->SetValue(GetStringValue(cal.offset));
        }
    }
    else
    {
        for (uint i = 0; i < RangeU::Count; i++)
        {
            const CalK &cal = DSet::Get(type, (RangeU::E)i);

            fields_U[i].value->SetValue(GetStringValue(cal.offset));
        }
    }
}


void WindowTableOffsets::FillK(TypeValue::E type, bool show_I)
{
    if (show_I)
    {
        for (uint i = 0; i < RangeI::Count; i++)
        {
            const CalK &cal = DSet::Get(type, (RangeI::E)i);

            fields_I[i].value->SetValue(GetStringValue(cal.k));
        }
    }
    else
    {
        for (uint i = 0; i < RangeU::Count; i++)
        {
            const CalK &cal = DSet::Get(CurrentMeasure(), (RangeU::E)i);

            fields_U[i].value->SetValue(GetStringValue(cal.k));
        }
    }
}


wxString WindowTableOffsets::GetStringValue(double value) const
{
    return wxString::Format("%.12f", value);
}


TypeValue::E WindowTableOffsets::CurrentMeasure() const
{
    struct StructButton
    {
        StructButton(wxRadioButton *b0, wxRadioButton *b1) : rb0{b0}, rb1{b1} { }

        wxRadioButton *rb0;
        wxRadioButton *rb1;
    };

    const StructButton buttons[TypeValue::Count] =
    {
        { rbChanC_MeasI,   rbChanC_MeasU },
        { rbChanC_MeasI,   rbChanC_MeasU },
        { rbChanB_MeasI,   rbChanB_MeasU },
        { rbChanB_SourceI, rbChanB_SourceU },
        { rbChanB_LimitI,  rbChanB_LimitU },
        { rbChanS_MeasI,   rbChanS_MeasU  },
        { rbChanS_SourceI, rbChanS_SourceU  },
        { rbChanS_LimitI,  rbChanS_LimitU }
    };

    for (int i = 0; i < TypeValue::Count; i++)
    {
        const wxRadioButton *rb0 = buttons[i].rb0;
        const wxRadioButton *rb1 = buttons[i].rb1;

        if (rb0->GetValue() ||
            rb1->GetValue())
        {
            return (TypeValue::E)i;
        }
    }

    LOG_ERROR("Can not find type measure");

    return TypeValue::Count;
}


void WindowTableOffsets::ShowFieldsI(bool show)
{
    using namespace Math;

    TypeValue::E type = CurrentMeasure();

    for (auto &field : fields_I)
    {
        field.name->Show(show);
        field.value->Show(show);

        if (show)
        {
            field.name->SetLabel(RangeI((RangeI::E)field.range).Name(CurrentMeasure() == TypeValue::Meas_C ? RowRange::_125 : RowRange::_124));

            bool condition = InRange<int>(field.range, RangeI::Min(type), RangeI::Max(type));

            if (!condition)
            {
                field.value->SetValue("");
            }

            field.name->Enable(condition);
            field.value->Enable(condition);
        }
    }
}


void WindowTableOffsets::ShowFieldsU(bool show)
{
    using namespace Math;

    TypeValue::E type = CurrentMeasure();

    for (auto &field : fields_U)
    {
        field.name->Show(show);
        field.value->Show(show);

        if (show)
        {
            field.name->SetLabel(RangeU((RangeU::E)field.range).Name(CurrentMeasure() == TypeValue::Meas_C ? RowRange::_125 : RowRange::_124));

            bool condition = InRange<int>(field.range, RangeU::Min(type), RangeU::Max(type));

            if (!condition)
            {
                field.value->SetValue("");
            }

            field.name->Enable(condition);
            field.value->Enable(condition);
        }
    }
}
