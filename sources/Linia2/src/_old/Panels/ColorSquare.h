// 2026/01/20 11:20:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ColorSquare : public wxPanel {
    wxColor m_color;

public:
    ColorSquare(wxWindow *parent, wxWindowID id,
        const wxColor &initialColor = *wxRED,
        const wxSize &size = wxSize(22, 22))
        : wxPanel(parent, id, wxDefaultPosition, size),
        m_color(initialColor)
    {

        // Устанавливаем минимальный размер
        SetMinSize(size);

        // Обработчик рисования
        Bind(wxEVT_PAINT, &ColorSquare::OnPaint, this);
    }

    // Установить цвет
    void SetColor(const wxColor &color)
    {
        m_color = color;
        Refresh();  // Перерисовать
    }

    // Получить текущий цвет
    wxColor GetColor() const
    {
        return m_color;
    }

private:
    void OnPaint(wxPaintEvent &)
    {
        wxPaintDC dc(this);
        DrawSquare(dc);
    }

    void DrawSquare(wxPaintDC &dc)
    {
        wxSize size = GetClientSize();

        // Устанавливаем цвет заливки
        dc.SetBrush(wxBrush(m_color));
        dc.SetPen(wxPen(*wxBLACK, 1));

        // Рисуем квадрат
        dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());
    }
};
