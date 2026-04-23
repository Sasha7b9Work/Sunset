// 2025/08/13 14:19:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Slider.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Configurator.h"
#pragma warning(push, 0)
#include <wx/slider.h>
#include <wx/stattext.h>
#pragma warning(pop)


SliderInt::SliderInt(wxWindow *parent, int width, int _min, int _max, const wxString &_name) :
    Panel(parent),
    min(_min),
    max(_max)
{
    Panel::SetSize({ width + 5, TEXTCNTRL_HEIGHT + 5 + 5 });

    Panel::SetName(parent->GetName() + "_" + _name);

    int w1 = 20;
    int w2 = 17;

    wxSize s{ width - w1 - w2, TEXTCNTRL_HEIGHT + 5 };

    slider = new wxSlider(this, wxID_ANY, min, min, max, { w1, 0 }, s );

    slider->SetMinSize(wxSize(100, 30));

    slider->SetBackgroundColour(parent->GetBackgroundColour());

    text = new wxStaticText(this, wxID_ANY, "0", { 0, 5 }, { w1, TEXTCNTRL_HEIGHT }, wxALIGN_RIGHT);

    wxSize size_button{ 15, 12 };

    int x = w1 + slider->GetSize().x;

    btnMore = new wxButton(this, wxID_ANY, "", { x, 0 }, size_button);
    btnMore->SetBackgroundColour(btnMore->GetBackgroundColour().ChangeLightness(LIGHTNESS));
    btnLess = new wxButton(this, wxID_ANY, "", { x, size_button.y }, size_button);
    btnLess->SetBackgroundColour(btnLess->GetBackgroundColour().ChangeLightness(LIGHTNESS));

    btnMore->SetCursor(wxCursor(wxCURSOR_HAND));
    btnLess->SetCursor(wxCursor(wxCURSOR_HAND));
    slider->SetCursor(wxCursor(wxCURSOR_HAND));

    slider->Bind(wxEVT_ENTER_WINDOW, &SliderInt::OnEventEnterWindow, this);

    // Устанавливать курсор будем так, потому что на Linux обычная установка не работает
    slider->Bind(wxEVT_SLIDER, &SliderInt::OnEventSlider, this);

    btnMore->Bind(wxEVT_LEFT_DOWN, &SliderInt::OnEventMouseDown, this);
    btnLess->Bind(wxEVT_LEFT_DOWN, &SliderInt::OnEventMouseDown, this);
//    btnMore->Bind(wxEVT_LEFT_UP, &SliderInt::OnEventMouseUp, this);
//    btnLess->Bind(wxEVT_LEFT_UP, &SliderInt::OnEventMouseUp, this);
}


void SliderInt::OnEventMouseDown(wxMouseEvent &event)
{
    if (event.GetId() == btnMore->GetId())
    {
        int new_value = slider->GetValue() + 1;
        if (new_value <= slider->GetMax())
        {
            text->SetLabel(wxString::Format("%d", new_value));
            slider->SetValue(new_value);
            GF::SendCommandEvent(this, wxEVT_SLIDER, new_value);
        }
    }
    else if (event.GetId() == btnLess->GetId())
    {
        int new_value = slider->GetValue() - 1;
        if (new_value >= slider->GetMin())
        {
            text->SetLabel(wxString::Format("%d", new_value));
            slider->SetValue(new_value);
            GF::SendCommandEvent(this, wxEVT_SLIDER, new_value);
        }
    }

    event.Skip();
}


void SliderInt::OnEventMouseUp(wxMouseEvent &event)
{
    timer_more.Stop();
    timer_less.Stop();

    event.Skip();
}


void SliderInt::OnEventSlider(wxCommandEvent &event)
{
    text->SetLabel(wxString::Format("%d", event.GetInt()));
    GF::SendCommandEvent(this, wxEVT_SLIDER, slider->GetValue());

    event.Skip();
}


void SliderInt::OnEventEnterWindow(wxMouseEvent &event)
{
    if (event.GetId() == slider->GetId())
    {
        slider->SetCursor(wxCursor(wxCURSOR_HAND));
    }

    event.Skip();
}


void SliderInt::SetValue(int value, bool send_event)
{
    if (value != GetValue())
    {
        slider->Unbind(wxEVT_SLIDER, &SliderInt::OnEventSlider, this);
        slider->SetValue(value);
        slider->Bind(wxEVT_SLIDER, &SliderInt::OnEventSlider, this);

        text->SetLabel(wxString::Format("%d", slider->GetValue()));

        if (send_event)
        {
            GF::SendCommandEvent(this, wxEVT_SLIDER, slider->GetValue());
        }
    }
}


void SliderInt::OnEventTimer(wxTimerEvent &event)
{
    int delta = (event.GetId() == timer_more.GetId()) ? 1 : -1;

    slider->SetValue(slider->GetValue() + delta);

    text->SetLabel(wxString::Format("%d", slider->GetValue()));

    GF::SendCommandEvent(this, wxEVT_SLIDER, slider->GetValue());

    event.Skip();
}


void SliderInt::Pack()
{
    Config::WriteInt(GetName(), GetValue());
}


void SliderInt::Unpack()
{
    int value = wxClip<int>(Config::ReadInt(GetName(), 0), min, max);

    SetValue(value);
}


void SliderFloat::Pack()
{
    Config::WriteInt(GetName(), GetIntValue());
}


void SliderFloat::Unpack()
{
    int value = wxClip<int>(Config::ReadInt(GetName()), slider->GetMin(), slider->GetMax());

    SetIntValue(value);
}


int SliderInt::GetValue() const
{
    return slider->GetValue();
}


int SliderFloat::GetIntValue() const
{
    return slider->GetValue();
}


void SliderFloat::SetIntValue(int value)
{
    GF::SendCommandEvent(this, wxEVT_SLIDER, value);
}


SliderFloat::SliderFloat(wxWindow *parent, int width, const wxString &name) :
    Panel(parent)
{
    Panel::SetSize({ width, TEXTCNTRL_HEIGHT + 5 + 5 });
    Panel::SetName(parent->GetName() + "_" + name);

    int w1 = 50;
    int w2 = 17;

    slider = new wxSlider(this, wxID_ANY, num_steps / 2, 0, num_steps, { w1, 0 }, { width - w1 - w2, TEXTCNTRL_HEIGHT + 5 });

    slider->SetBackgroundColour(slider->GetBackgroundColour().ChangeLightness(LIGHTNESS));

    text = new wxStaticText(this, wxID_ANY, "0", { 0, 5 }, { w1, TEXTCNTRL_HEIGHT - 5 });

    wxSize size_button{ 15, 12 };

    int x = w1 + slider->GetSize().x;

    btnMore = new wxButton(this, wxID_ANY, "", { x, 0 }, size_button);
    btnMore->SetBackgroundColour(btnMore->GetBackgroundColour().ChangeLightness(LIGHTNESS));
    btnLess = new wxButton(this, wxID_ANY, "", { x, size_button.y }, size_button);
    btnLess->SetBackgroundColour(btnLess->GetBackgroundColour().ChangeLightness(LIGHTNESS));

    Bind(wxEVT_SLIDER, &SliderFloat::OnEventSlider, this);
    btnMore->Bind(wxEVT_LEFT_DOWN, &SliderFloat::OnEventMouseDown, this);
    btnLess->Bind(wxEVT_LEFT_DOWN, &SliderFloat::OnEventMouseDown, this);
    btnMore->Bind(wxEVT_LEFT_UP, &SliderFloat::OnEventMouseUp, this);
    btnLess->Bind(wxEVT_LEFT_UP, &SliderFloat::OnEventMouseUp, this);

    Bind(wxEVT_TIMER, &SliderFloat::OnEventTimer, this);
    Bind(wxEVT_TIMER, &SliderFloat::OnEventTimer, this);
}


void SliderFloat::SetRange(double _min, double _max, const wxString &_units, int _digits_after_point)
{
    digitts_after_points = _digits_after_point;
    units = _units;
    min = _min;
    max = _max;

    step = (max - min) / num_steps;

    CalculateValue();
}


void SliderFloatOffset::CalculateAndSetRange(const wxString &range, double multiplier)
{
    wxString value = range.BeforeFirst(' ');

    units = range.AfterFirst(' ');

    int int_value = 0;
    value.ToInt(&int_value);

    double double_value = int_value * multiplier;

    int digits = 0;

    if (double_value <= 1000)
    {
        if (double_value <= 50)
        {
            digits = 1;
        }
        else
        {
            digits = 0;
        }
    }
    else
    {
        double_value *= 1e-3;

        if (units[0] == 'A' || units[0] == 'V') units = 'k' + units;
        else if (units[0] == 'm')               units = units[1];
        else if (units[0] == 'u')               units[0] = 'm';
        else if (units[0] == 'n')               units[0] = 'u';
        else if (units[0] == 'p')               units[0] = 'n';

        digits = 2;
    }

    SetRange(0.0, double_value, units, digits);
}


void SliderFloatLimit::CalculateAndSetRange(const wxString &range)
{
    wxString value = range.BeforeFirst(' ');

    units = range.AfterFirst(' ');

    int int_value = 0;
    value.ToInt(&int_value);

    double double_value = (double)int_value;

    int digits = 0;

    if (double_value <= 1000)
    {
        if (double_value <= 5)          digits = 2;
        else if (double_value <= 50)    digits = 1;
        else                            digits = 0;
    }
    else
    {
        double_value *= 1e-3;

        if (units[0] == 'A' || units[0] == 'V') units = 'k' + units;
        else if (units[0] == 'm')               units = units[1];
        else if (units[0] == 'u')               units[0] = 'm';
        else if (units[0] == 'n')               units[0] = 'u';
        else if (units[0] == 'p')               units[0] = 'n';

        digits = 2;
    }

    SetRange(double_value / 10.0, double_value * 1.1, units, digits);
}


void SliderFloatPercents::CalculateValue()
{
    SliderFloat::CalculateValue();

    double value = min + (max - min) * slider->GetValue() / num_steps;

    value = (double)max_percents / max * value;

    textPercents->SetLabel(wxString::Format("%.0f %%", value));
}


void SliderFloat::CalculateValue()
{
    double value = min + (max - min) * slider->GetValue() / num_steps;

    char format_string[32];
    std::sprintf(format_string, "%%.%df %%s", digitts_after_points);

    text->SetLabel(wxString::Format(format_string, value, units.c_str().AsChar()));
}


void SliderFloat::OnEventSlider(wxCommandEvent &event)
{
    int value = event.GetInt();

    if (slider->GetValue() != value)
    {
        slider->SetValue(value);
    }

    CalculateValue();

    event.Skip();
}


void SliderFloat::OnEventMouseDown(wxMouseEvent &event)
{
    if (event.GetId() == btnMore->GetId())
    {
        slider->SetValue(slider->GetValue() + 1);
        timer_more.Start(400);
    }
    else if (event.GetId() == btnLess->GetId())
    {
        slider->SetValue(slider->GetValue() - 1);
        timer_less.Start(400);
    }

    GF::SendCommandEvent(this, wxEVT_SLIDER, slider->GetValue());

    event.Skip();
}


void SliderFloat::OnEventMouseUp(wxMouseEvent &event)
{
    timer_more.Stop();
    timer_less.Stop();

    event.Skip();
}


void SliderFloat::OnEventTimer(wxTimerEvent &event)
{
    int delta = (event.GetId() == timer_more.GetId()) ? 1 : -1;

    slider->SetValue(slider->GetValue() + delta);

    GF::SendCommandEvent(this, wxEVT_SLIDER, slider->GetValue());

    event.Skip();
}


SliderFloatPercents::SliderFloatPercents(wxWindow *parent, int width, int _min_percents, int _max_percents, const wxString &name) :
    SliderFloat(parent, width, name),
    min_percents(_min_percents),
    max_percents(_max_percents)
{
    text->SetPosition({0, 0});

    textPercents = new wxStaticText(this, wxID_ANY, "0", wxDefaultPosition, { 50, TEXTCNTRL_HEIGHT - 5 });
}
