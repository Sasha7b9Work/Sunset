// 2026/03/09 10:24:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/dcbuffer.h>
#pragma warning(pop)


class KnobWidget : public wxControl
{
public:
    KnobWidget(wxWindow *parent, int minValue, int maxValue, int initialValue)
        : wxControl(parent, wxID_ANY, wxDefaultPosition, {70, 50}, wxBORDER_NONE),
        minValue(minValue),
        maxValue(maxValue),
        value(initialValue)
    {
        wxWindowBase::SetBackgroundColour(parent->GetBackgroundColour());

        // Включаем обработку событий мыши
        Bind(wxEVT_LEFT_DOWN, &KnobWidget::OnEventMouseLeftDown, this);
        Bind(wxEVT_LEFT_UP, &KnobWidget::OnMouseLeftUp, this);
        Bind(wxEVT_MOTION, &KnobWidget::OnMouseMove, this);
        Bind(wxEVT_MOUSE_CAPTURE_LOST, &KnobWidget::OnMouseCaptureLost, this);
        Bind(wxEVT_PAINT, &KnobWidget::OnPaint, this);
        Bind(wxEVT_SET_FOCUS, &KnobWidget::OnSetFocus, this);
        Bind(wxEVT_KILL_FOCUS, &KnobWidget::OnKillFocus, this);

        wxWindowBase::SetCursor(wxCursor(wxCURSOR_HAND));

        // Включаем двойную буферизацию для устранения мерцания
        wxWindowBase::SetBackgroundStyle(wxBG_STYLE_PAINT);
    }

    int GetValue() const
    {
        return value;
    }

    int GetMinValue() const
    {
        return minValue;
    }

    int GetMaxValue() const
    {
        return maxValue;
    }

    void SetValue(int _value, bool _send_event = true)
    {
        if (_value < minValue) value = minValue;
        if (_value > maxValue) value = maxValue;

        if (value != _value)
        {
            value = _value;
            Refresh();

            if (_send_event)
            {
                // Отправляем событие об изменении значения
                wxCommandEvent event(wxEVT_SLIDER, GetId());
                event.SetInt(value);
                event.SetEventObject(this);
                GetEventHandler()->ProcessEvent(event);
            }
        }
    }

private:
    int minValue;
    int maxValue;
    int value;
    bool dragging = false;
    wxPoint capturePoint; // Точка, где был захвачен виджет
    wxPoint globalCapturePoint;

    void OnEventMouseLeftDown(wxMouseEvent &event)
    {
        if (!dragging)
        {
            dragging = true;
            capturePoint = event.GetPosition(); // Запоминаем позицию захвата
            globalCapturePoint = ClientToScreen(capturePoint);
            CaptureMouse(); // Захватываем мышь для получения событий вне виджета

            wxWindowBase::SetCursor(wxCursor(wxBitmap{ 1, 1, 1 }));
        }
        event.Skip();
    }

    void OnMouseLeftUp(wxMouseEvent &event)
    {
        if (dragging)
        {
            dragging = false;
            if (HasCapture())
                ReleaseMouse();

            wxWindowBase::SetCursor(wxCursor(wxCURSOR_HAND));
        }
        event.Skip();
    }

    void OnMouseMove(wxMouseEvent &event)
    {
        if (dragging && event.LeftIsDown())
        {
            // ВАЖНО: Возвращаем курсор мыши в исходную позицию
            // Это создает эффект, что курсор остается на месте при вращении
            WarpPointer(capturePoint.x, capturePoint.y);

            wxPoint currentPos = event.GetPosition();

            static float delta = 0.0f;

            // Вычисляем дельту относительно точки захвата
            float deltaY = (float)(currentPos.y - capturePoint.y) / 10.0f;
            float deltaX = (float)(-currentPos.x + capturePoint.x) / 10.0f;

            delta += (deltaX + deltaY);

            if ((int)delta != 0)
            {
                // Изменяем значение в зависимости от направления движения
                // Тянем вверх (отрицательный deltaY) - увеличиваем значение
                // Тянем вниз (положительный deltaY) - уменьшаем значение
                int newValue = (int)((float)value - (float)((int)delta)); // Минус потому что Y увеличивается вниз

                // Ограничиваем значение
                if (newValue < minValue) newValue = minValue;
                if (newValue > maxValue) newValue = maxValue;

                SetValue(newValue);

                delta -= (float)((int)delta);
            }
        }
        event.Skip();
    }

    void OnMouseCaptureLost(wxMouseCaptureLostEvent &)
    {
        dragging = false;
    }

    void OnSetFocus(wxFocusEvent &event)
    {
        Refresh();
        event.Skip();
    }

    void OnKillFocus(wxFocusEvent &event)
    {
        Refresh();
        event.Skip();
    }

    void OnPaint(wxPaintEvent &)
    {
        SetBackgroundColour(GetParent()->GetBackgroundColour());

        wxAutoBufferedPaintDC dc(this);
        dc.Clear();

        wxSize size = GetClientSize();
        int width = size.GetWidth();
        int height = size.GetHeight();
        int diameter = std::min(width, height) - 4;
        int x = (width - diameter) / 2;
        int y = (height - diameter) / 2;

        // Рисуем фон
        dc.SetBrush(wxBrush(GetBackgroundColour()));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(0, 0, width, height);

        // Рисуем корпус ручки
        dc.SetBrush(wxBrush(wxColour(80, 80, 80))); // Тёмно-серый
        dc.SetPen(wxPen(wxColour(120, 120, 120), 1));
        dc.DrawEllipse(x + 10, y, diameter, diameter);

        // Рисуем блик
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(wxPen(wxColour(220, 220, 220), 1));
        dc.DrawEllipse(x + 2 + 10, y + 2, diameter - 4, diameter - 4);

        // Вычисляем угол поворота (от -210° до +90°, диапазон 300°)
        double angle = (double)(value - minValue) / (maxValue - minValue) * 300.0 - 210.0;
        angle = angle * M_PI / 180.0; // Конвертируем в радианы

        // Рисуем указатель
        int centerX = x + diameter / 2 + 10;
        int centerY = y + diameter / 2;
        int radius = diameter / 2 - 6;

        int indicatorX = centerX + (int)(radius * cos(angle));
        int indicatorY = centerY + (int)(radius * sin(angle));

        dc.SetPen(wxPen(wxColour(255, 200, 0), 3)); // Жёлтый указатель
        dc.DrawLine(centerX, centerY, indicatorX, indicatorY);

        // Рисуем центральную точку
        dc.SetBrush(wxBrush(wxColour(255, 200, 0)));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawCircle(centerX, centerY, 3);

        // Опционально: рисуем метки минимального и максимального значения
        dc.SetPen(wxPen(wxColour(200, 200, 200), 1));

        // Метка минимума
        int minX = 0;
        int minY = GetClientSize().y - 15;
        dc.DrawText(wxString::Format("%d", minValue), minX, minY);

        dc.DrawText(wxString::Format("%d", value), 0, GetClientSize().y / 2 - 7);

        // Метка максимума (справа)
        wxString maxStr = wxString::Format("%d", maxValue);
        int textWidth, textHeight;
        dc.GetTextExtent(maxStr, &textWidth, &textHeight);
        dc.DrawText(maxStr, 0, 0);
    }
};
