// 2025/6/4 15:50:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PainterRegister.h"
#include "Panels/PanelDebug/Notebook/Register.h"
#include "Panels/PanelDebug/Notebook/AnimatedImpulse.h"
#pragma warning(push, 0)
#include <wx/graphics.h>
#include <wx/sizer.h>
#pragma warning(pop)


PainterRegister::PainterRegister(wxWindow *parent, Register *_reg) :
    Panel(parent),
    reg(_reg)
{
    Panel::SetMinSize({ 750, 110 });

    Panel::SetBackgroundColour(GetBackgroundColour().ChangeLightness(150));

    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);

    reg->chboxes.resize((uint)_reg->chip->BitDepth() );

    wxBoxSizer *hor_sizer = new wxBoxSizer(wxHORIZONTAL);                  // Здесь будут находиться чекбоксы битов и AnimatedImpulse

    {
        wxBoxSizer *checkBoxSizer = new wxBoxSizer(wxHORIZONTAL);
        checkBoxSizer->AddSpacer(36);

        for (int i = 0; i < reg->chip->BitDepth(); i++)
        {
            CheckBoxBit *chb = new CheckBoxBit(this, { W_B, W_B });
            reg->chboxes[(uint)i] = chb;
        }

        for (int i = reg->chip->BitDepth() - 1; i >= 0; i--)
        {
            checkBoxSizer->Add(reg->chboxes[(uint)i], 0, wxALIGN_CENTER_VERTICAL, 0);
        }

        checkBoxSizer->AddStretchSpacer(1);

        wxBoxSizer *vert_sizer = new wxBoxSizer(wxVERTICAL);
        vert_sizer->AddSpacer(45);
        vert_sizer->Add(checkBoxSizer);
        vert_sizer->AddStretchSpacer();

        hor_sizer->Add(vert_sizer);
    }

    {
        wxBoxSizer *ver_sizer = new wxBoxSizer(wxVERTICAL);
        ver_sizer->AddStretchSpacer(1);

        animation = new AnimatedImpulse(this, ColorBackground(true));
        ver_sizer->Add(animation, 0, wxRIGHT | wxBOTTOM, 5);

        hor_sizer->AddStretchSpacer(1);

        hor_sizer->Add(ver_sizer, 0, wxEXPAND | wxRIGHT | wxBOTTOM);
    }

    main_sizer->Add(hor_sizer, 1, wxEXPAND);
    SetSizer(main_sizer);

    Bind(wxEVT_PAINT, &PainterRegister::OnEventPaint, this);
}


void PainterRegister::IncreaseHeight(int dH)
{
    wxSize size = GetSize();

    size.y += dH;

    SetMinSize(size);
    SetMaxSize(size);
    SetSize(size);

    Layout();

    Refresh();
}


int PainterRegister::BitX(int num_bit, int all_bits) const
{
    int rigth = 40 + all_bits * W_B;

    return rigth - (num_bit + 1) * W_B - 4;
}


wxColor PainterRegister::ColorBackground(bool enabled) const
{
    return enabled ? GetParent()->GetBackgroundColour().ChangeLightness(170) : GetParent()->GetBackgroundColour();
}


void PainterRegister::OnEventPaint(wxPaintEvent &)
{
    if (first_paint)
    {
        first_paint = false;

        for (int i = 0; i < reg->chip->BitDepth(); i++)
        {
            SetHintCheckBox(i);
        }
    }

    wxPaintDC _dc(this);

    wxGraphicsContext *gc = wxGraphicsContext::Create(_dc);

    gc->SetPen(*wxGREEN_PEN);

    gc->SetBrush(ColorBackground(IsEnabled()));

    gc->DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);

    gc->SetPen(*wxBLACK_PEN);

    int num_bits = reg->chip->BitDepth();

    int w = 20;
    int h = w;

    bool pen_is_white = false;           // Признак того, что заполнение идёт белым

    for (int i = 0; i < num_bits; i++)
    {
        if ((i % 8) == 0)
        {
            wxBrush brush(wxColor(0xF0, 0xF0, 0xF0));

            gc->SetBrush(IsEnabled() ? (pen_is_white ? brush : *wxWHITE_BRUSH) : ColorBackground(IsEnabled()));
            pen_is_white = !pen_is_white;
        }

        wxPoint coord = CoordBit(i);

        gc->DrawRectangle(coord.x, coord.y, w, h);
    }

    for (int i = 0; i < num_bits; i++)
    {
        wxPoint coord = CoordBit(i);

        // Рисуем надпись над чекбоксом (y = 2, а не coord.y + d)
        int y = 30;  // Фиксированная позиция сверху
        int width = W_B;

        DrawTextInCenter(coord.x, y, width, reg->names_bits[(uint)i], 7, gc);
    }

    gc->SetBrush(IsEnabled() ? *wxWHITE_BRUSH : ColorBackground(IsEnabled()));

    for (int i = 0; i < 2; i++)
    {
        DrawDescriptions(i, gc);
    }

    gc->SetFont(GetDefaultFont(8), *wxBLACK);

    int y = 30;
    gc->DrawText("DB0", W_B * reg->chip->BitDepth() + 45, y);
    gc->DrawText(wxString::Format("DB%d", reg->chip->BitDepth() - 1), 5, y);

    delete gc;
}


void PainterRegister::SetHintCheckBox(int num_bit)
{
    wxString hint = reg->names_bits[(uint)num_bit];

    wxString desc0 = GetHint(0, num_bit);

    if (!desc0.empty())
    {
        hint += "\n";
        hint += desc0;
    }

    wxString desc1 = GetHint(1, num_bit);

    if (!desc1.empty())
    {
        hint += "\n";
        hint += "Descriptin 2 : ";
        hint += desc1;
    }

    reg->chboxes[(uint)num_bit]->SetToolTip(hint);
}


wxPoint PainterRegister::CoordBit(int num_bit)
{
    num_bit = reg->chip->BitDepth() - num_bit - 1;

    return { 36 + num_bit * W_B, 25 };
}


void PainterRegister::DrawDescriptions(int index, wxGraphicsContext *gc)
{
    std::vector<StructDescription> &desc = reg->desc[index];

    for (uint i = 0; i < desc.size(); i++)
    {
        StructDescription &d = desc[i];

        if (d.desc[0])
        {
            wxPoint coord = CoordBit(d.first_bit);

            int x = coord.x;
            x -= (d.num_bits - 1) * W_B;
            int y = coord.y + 41 + index * W_B;
            int w = W_B * d.num_bits;
            int h = W_B;

            gc->DrawRectangle(x, y, w, h);

            DrawTextInCenter(x, y + 4, w, d.desc, 8, gc);
        }
    }
}


wxFont PainterRegister::GetDefaultFont(int size)
{
    wxFont font(7, wxFONTFAMILY_DEFAULT,
        wxFONTSTYLE_NORMAL,
        wxFONTWEIGHT_BOLD);

    if (size != -1)
    {
        font.SetPointSize(size);
    }

    return font;
}


void PainterRegister::DrawTextInCenter(int x, int y, int width, const wxString &text, int size, wxGraphicsContext *gc)
{
    gc->SetFont(GetDefaultFont(size), *wxBLACK);

    wxDouble textWidth = 10000;
    wxDouble textHeight = 0;

    while (textWidth >= width)
    {
        gc->GetTextExtent(text, &textWidth, &textHeight);

        if (textWidth < width - 2)
        {
            break;
        }

        size--;
        gc->SetFont(GetDefaultFont(size), *wxBLACK);
    }

    gc->DrawText(text, x + width / 2 - textWidth / 2 + 1, y);
}


wxString PainterRegister::GetHint(int index_desc, int num_bit)
{
    if (reg->desc[index_desc].empty())
    {
        return "";
    }

    std::vector<StructDescription> &desc = reg->desc[index_desc];

    for (uint i = 0; i < desc.size(); i++)
    {
        if (num_bit >= desc[i].first_bit)
        {
            if (num_bit < desc[i].first_bit + desc[i].num_bits)
            {
                return desc[i].hint;
            }
        }
    }

    return "";
}
