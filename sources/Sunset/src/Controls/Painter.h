// 2023/09/02 18:48:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Bitmap.h"


struct Color;


class Painter : public wxPanel
{
public:

    Painter(wxWindow *parent, const wxPoint &position, const wxSize &size);

    virtual void OnPaint(wxPaintEvent &);

    void BeginPaint(const wxColor &background = wxNullColour);
    void EndPaint();
    void FillCircle(int x, int y, int r, const wxColor & = wxNullColour);
    void DrawCircle(int x, int y, int r, const wxColor & = wxNullColour);
    void FillRectangle(int x, int y, int w, int h, const wxColor & = wxNullColour);
    void DrawRectangle(int x, int y, int w, int h, const wxColor & = wxNullColour);
    void DrawBitmap(int x, int y, const wxBitmap &, const wxColor &alpha = wxNullColour);

    wxGraphicsContext *gc = nullptr;

protected:

    wxBitmap *bitmap = nullptr;
    wxMemoryDC dc;
    wxColor color;
    wxSize size;
};


// Тупо заливает на себе прямоугольник
class PainterRect : public wxPanel
{
public:

    PainterRect(wxWindow *parent, const wxPoint &position, const wxSize &size);

    virtual void OnPaint(wxPaintEvent &);

    void SetColor(const Color &);

    wxColour GetColor() const { return color; }

private:

    wxColour color;
};



// Отображает BMP
class PainterBMP : public wxPanel
{
public:

    // Если transparentColour != nullptr, то пиксели данного цвета будут прозрачными
    PainterBMP(wxWindow *, const wxPoint &, const wxSize &, const wxString &file_name, const wxColour &alpha = wxNullColour);

private:

    Bitmap bitmap;

    void OnEventPaint(wxPaintEvent &);
};


class PainterAnimated : public Painter
{
public:

    PainterAnimated(wxWindow *parent, const wxPoint &position, const wxSize &);

protected:

    wxTimer timer;

private:

    void OnEventPaint(wxPaintEvent &);
    void OnEventTimer(wxTimerEvent &);

    virtual void FuncDraw() = 0;
};
