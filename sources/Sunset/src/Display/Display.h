// 2022/10/28 23:17:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Grid/Grid.h"
#include "Display/PanelErrors.h"
#include "Display/Graphics/GraphMeasure.h"
#include "Panels/Panels.h"


// Здесь отрисовывается график ну и кнопки разные


class Display : public Panel
{
    friend class Point;
    friend class Text;
    friend class Spline;
    friend class Line;
    friend class Rect;
    friend class MenuDisplay;
    friend class GraphMeasure;
    friend class MainWindow;

public:

    ~Display();

    void SetColorBrush(const wxColor &);
    void SetColorPen(const wxColor &);
    void LoadColors();

    bool mouse_is_pressed = false;
    bool full_screen = false;       // График рисуется на полный экран

    PanelErrors *panel_errors = nullptr;

    void FullScreen(bool);

    void FillRectangle(int x, int y, int width, int height, const wxColor &);

    void OnEventCnangeMeasuredElement();

    // Возвращает размер области для отрисовки
    wxSize GetDrawingSize() const;

    // Возврщает полнлный размер виджета
    wxSize GetFullSize() const;

    std::vector<GraphMeasure *> entities;    // Сущности для отрисовки

    void ReInit();

private:

    Display(wxWindow *);

    wxButton *btnHelp = nullptr,
        *btnLessX = nullptr,
        *btnMoreX = nullptr,
        *btnLessY = nullptr,
        *btnMoreY = nullptr;

    std::vector<wxButton *> buttons;        // Здесь все кнопки, у которых нужно менять цвета

    wxBitmap *bitmap = nullptr;
    wxGraphicsContext *gc = nullptr;        // Используется для рисования
    wxMemoryDC dc;                          // А здесь хранится то, что нарисовано, пока не будет вызвано событие wxEVT_PAINT
    wxColor color_pen;
    wxColor color_brush;

    // Координаты мыши при нажатии кнопки
    wxPoint pos_mouse_down;

    void Init();

    void BeginPaint();

    void EndPaint();

    void OnEventRightClick(wxMouseEvent &);
    void OnEventPaint(wxPaintEvent &);
    void OnEventMouseDown(wxMouseEvent &);
    void OnEventMouseUp(wxMouseEvent &);
    void OnEventMouseMove(wxMouseEvent &);
    void OnEventMouseWheel(wxMouseEvent &);
    void OnEventLeaveWindow(wxMouseEvent &);
    void OnEventEnterWindow(wxMouseEvent &);
    void OnEventButton(wxCommandEvent &);

    wxSize GetSize() const;
    wxSize GetClientSize() const;
};


class Point
{
public:

    void Draw(int x, int y) const;
};


class Line
{
public:

    Line(int _x1, int _y1, int _x2, int _y2) :
        x1(_x1), y1(_y1), x2(_x2), y2(_y2) { }

    void Draw() const;
    void Draw(const wxColor &) const;

private:
    int x1, y1, x2, y2;
};


class Rect
{
public:

    Rect(int _w, int _h) : width(_w), height(_h) { }

    void Fill(int x, int y, const wxColor &) const;
    void Draw(int x, int y, const wxColor &) const;

private:
    int width, height;
};


class Text
{
public:

    Text(const wxString &);

    static void SetFont();

    void Draw(int x, int y) const;

    // Рисует слева от точки по центру
    void DrawAboutCenterLeft(int x, int y, bool fillBackground) const;

    void DrawAboutCenterRigth(int x, int y, bool fillBackground) const;

    // Рисует снизу по центру
    void DrawAboutCenterDown(int x, int y, bool fillBackground) const;

    void DrawAboutCenterUp(int x, int y, bool fillBackground) const;

    // Если frame, то вокруг нужно нарисовать рамку
    void DrawAboutRightUp(int x, int y, bool fillBackground, bool frame = false) const;

private:

    wxString text;
};


class Spline
{
public:

    // smooth - сглаживание
    void Draw(const std::vector<wxPoint> &, bool smooth, bool draw_points) const;
};
