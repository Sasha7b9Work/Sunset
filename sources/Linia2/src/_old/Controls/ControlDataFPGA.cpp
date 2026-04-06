// 2026/04/01 14:24:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ControlDataFPGA.h"


PainterDataFPGA::PainterDataFPGA(wxWindow *parent, const wxSize &size) :
    Painter(parent, { 0, 0 }, size)
{
}


ControlDataFPGA::ControlDataFPGA(wxWindow *parent, const wxPoint &position) :
    wxPanel(parent, wxID_ANY, position, SIZE, wxBORDER_SIMPLE)
{
    SetSize(SIZE);

    wxSize size{ SIZE };
    size.x -= 2;
    size.y -= 2;

    painter = new PainterDataFPGA(this, size);

    painter->SetPosition({ 0, 0 });

    btnScale = new wxToggleButton(painter, wxID_ANY, "S", { size.x - 17, 0 }, { 17, 17 });

    btnScale->SetToolTip("Изменение масштаба - автоматический или постоянный");

    SetMax((1 << 18) - 1);

    Draw();

    Bind(wxEVT_TOGGLEBUTTON, &ControlDataFPGA::OnEventToggleButon, this);
}


void ControlDataFPGA::OnEventToggleButon(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnScale->GetId())
    {
        Draw();
    }
}


void ControlDataFPGA::SetMax(int _max)
{
    max = _max;

    for (int i = 0; i < MAX_NUMBER_POINTS; i++)
    {
        data[i] = _max / 2;
    }

    Draw();
}


void ControlDataFPGA::Draw()
{
    painter->BeginPaint(*wxWHITE);

    painter->gc->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), *wxBLACK);

    int min_value = 0;
    int max_value = max;

    if (btnScale->GetValue())
    {
        min_value = max;
        max_value = 0;

        for (int i = 0; i < MAX_NUMBER_POINTS; i++)
        {
            if (data[i] < min_value)
            {
                min_value = data[i];
            }
            if (data[i] > max_value)
            {
                max_value = data[i];
            }
        }
    }

    painter->gc->DrawText(wxString::Format("%d", min_value), {0.0, (double)(painter->GetSize().y - 14)});

    painter->gc->DrawText(wxString::Format("%d", max_value), {0.0, -3.0});

    painter->gc->SetPen(*wxBLACK_PEN);
    painter->gc->SetBrush(*wxBLACK_BRUSH);

    float scale_y = (float)(painter->GetSize().y - 4) / (float)(max_value - min_value);

    for (int i = 0; i < painter->GetSize().x; i++)
    {
        int y = (int)((float)(data[i] - min_value) * scale_y) + 2;

        painter->gc->DrawRectangle(i, painter->GetSize().y - y - 1, 1, 1);
    }

    painter->EndPaint();
}


void ControlDataFPGA::SetData(int _data[MAX_NUMBER_POINTS])
{
    std::memcpy(data, _data, sizeof(data[0]) * MAX_NUMBER_POINTS);

    Draw();
}
