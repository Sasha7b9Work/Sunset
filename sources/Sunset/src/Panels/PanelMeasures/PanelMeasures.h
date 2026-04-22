// 2022/10/28 23:17:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelMeasures/Grid/Grid.h"
#include "Panels/PanelMeasures/PanelErrors.h"
#include "Panels/PanelMeasures/Graphics/GraphMeasure.h"
#include "Panels/Panels.h"


class Notebook;


// Здесь отрисовывается график ну и кнопки разные

class PanelMeasures : public Panel
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

    ~PanelMeasures();

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

private:

    PanelMeasures(Notebook *, PanelMeasures *&);

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

    virtual void CallbackOnEventSize() override;

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
