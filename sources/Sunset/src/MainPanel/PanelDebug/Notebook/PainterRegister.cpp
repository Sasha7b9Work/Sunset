// 2025/6/4 15:50:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainPanel/PanelDebug/Notebook/PainterRegister.h"
#include "MainPanel/PanelDebug/Notebook/Register.h"
#include "MainPanel/PanelDebug/Notebook/AnimatedImpulse.h"
#pragma warning(push, 0)
#include <wx/graphics.h>
#pragma warning(pop)


PainterRegister::PainterRegister(wxWindow *parent, Register *_panel, const wxPoint &position) :
    Panel(parent),
    panel(_panel)
{
    Panel::SetPosition(position);
    Panel::SetSize({ 750, 110 });

    Panel::SetBackgroundColour(GetBackgroundColour().ChangeLightness(150));

    panel->chboxes.resize((uint)_panel->chip->BitDepth() );

    for (int i = 0; i < panel->chip->BitDepth(); i++)
    {
        panel->chboxes[(uint)i] = new CheckBoxBit(this, { BitX(i, panel->chip->BitDepth()), W_B + 1 }, { W_B, W_B });
    }

    Bind(wxEVT_PAINT, &PainterRegister::OnEventPaint, this);

    animation = new AnimatedImpulse(this, ColorBackground(true));

    SetPositionAnimationWidget();
}


void PainterRegister::IncreaseHeight(int dH)
{
    wxSize size = GetSize();

    size.y += dH;

    SetMinSize(size);
    SetMaxSize(size);
    SetSize(size);

    Layout();

    SetPositionAnimationWidget();

    Refresh();
}


void PainterRegister::SetPositionAnimationWidget()
{
    wxPoint pos = { GetSize().x, GetSize().y };
    pos.x -= AnimatedImpulse::WIDTH + 5;
    pos.y -= AnimatedImpulse::HEIGHT + 5;
    animation->SetPosition(pos);
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

        for (int i = 0; i < panel->chip->BitDepth(); i++)
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

    int num_bits = panel->chip->BitDepth();

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
        DrawTitleBit(i, panel->names_bits[(uint)i], gc);
    }

    gc->SetBrush(IsEnabled() ? *wxWHITE_BRUSH : ColorBackground(IsEnabled()));

    for (int i = 0; i < 2; i++)
    {
        DrawDescriptions(i, gc);
    }

    gc->SetFont(GetDefaultFont(8), *wxBLACK);

    int y = 24;
    gc->DrawText("DB0", W_B * panel->chip->BitDepth() + 45, y);
    gc->DrawText(wxString::Format("DB%d", panel->chip->BitDepth() - 1), 5, y);

    delete gc;
}


void PainterRegister::SetHintCheckBox(int num_bit)
{
    wxString hint = panel->names_bits[(uint)num_bit];

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

    panel->chboxes[(uint)num_bit]->SetToolTip(hint);
}


wxPoint PainterRegister::CoordBit(int num_bit)
{
    num_bit = panel->chip->BitDepth() - num_bit - 1;

    return { 36 + num_bit * 20, 0 };
}


void PainterRegister::DrawDescriptions(int index, wxGraphicsContext *gc)
{
    std::vector<StructDescription> &desc = panel->desc[index];

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


void PainterRegister::DrawTitleBit(int num_bit, const wxString &title, wxGraphicsContext *gc)
{
    wxPoint coord = CoordBit(num_bit);

    int d = (num_bit % 2) ? 2 : 6;

    DrawTextInCenter(coord.x, coord.y + d, W_B, title, 7, gc);
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
    if (panel->desc[index_desc].empty())
    {
        return "";
    }

    std::vector<StructDescription> &desc = panel->desc[index_desc];

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
