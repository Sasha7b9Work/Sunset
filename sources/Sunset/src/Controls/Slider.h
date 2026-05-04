// 2025/08/13 14:18:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Panel.h"
#pragma warning(push, 0)
#include <wx/timer.h>
#pragma warning(pop)


class wxSlider;
class Button;
class wxStaticText;


class SliderInt : public Panel
{
public:

    SliderInt(wxWindow *parent, int width, int min, int max, const wxString &name);

    void Pack();
    void Unpack();

    int GetValue() const;
    void SetValue(int, bool send_event = true);

private:

    int m_min = 0;
    int m_max = 0;

    wxStaticText *text = nullptr;
    wxSlider     *slider = nullptr;
    Button       *btnMore = nullptr;
    Button       *btnLess = nullptr;

    wxTimer timer_less;
    wxTimer timer_more;

    void OnEventSlider(wxCommandEvent &);
    void OnEventMouseDown(wxMouseEvent &);
    void OnEventMouseUp(wxMouseEvent &);
    void OnEventTimer(wxTimerEvent &);
    void OnEventEnterWindow(wxMouseEvent &);
};


class SliderFloat : public Panel
{
public:

    SliderFloat(wxWindow *parent, int width, const wxString &name);

    void SetRange(double min, double max, const wxString &units, int digits_after_point);

    void Pack();
    void Unpack();

    int GetIntValue() const;
    void SetIntValue(int);

protected:

    wxStaticText *text = nullptr;
    wxSlider     *slider = nullptr;
    Button     *btnMore = nullptr;
    Button     *btnLess = nullptr;

    wxTimer timer_less;
    wxTimer timer_more;

    int digitts_after_points = 0;
    wxString units;
    double min = 0.0;
    double max = 0.0;
    double step = 0.0f;
    const int num_steps = 100;

    void OnEventSlider(wxCommandEvent &);
    void OnEventMouseDown(wxMouseEvent &);
    void OnEventMouseUp(wxMouseEvent &);
    void OnEventTimer(wxTimerEvent &);

    // Рассчитать и показать новое значение
    virtual void CalculateValue();
};


class SliderFloatPercents : public SliderFloat
{
public:

    SliderFloatPercents(wxWindow *parent, int width, int min_percents, int max_percents, const wxString &name);

    virtual void CalculateValue() override;

protected:

    int min_percents = 0;
    int max_percents = 0;

private:

    wxStaticText *textPercents = nullptr;
};


class SliderFloatOffset : public SliderFloatPercents
{
public:

    SliderFloatOffset(wxWindow *parent, int width, const wxString &name) :
        SliderFloatPercents(parent, width, 0, 100, name) { }

    void CalculateAndSetRange(const wxString &range, double multiplier);
};


class SliderFloatLimit : public SliderFloatPercents
{
public:

    SliderFloatLimit(wxWindow *parent, int width, const wxString &name) :
        SliderFloatPercents(parent, width, 10, 110, name) { }

    void CalculateAndSetRange(const wxString &range);
};
