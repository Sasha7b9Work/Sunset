// 2022/10/28 23:17:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "MainWindow.h"
#include "MainWindow.h"
#include "Display/Graphics/GraphMeasure.h"
#include "Display/MenuDisplay.h"
#include "Display/Graphics/Splines.h"
#include "Settings/Settings.h"
#include "Utils/Timer.h"
#include "Display/Graphics/AutoCursors.h"


Display *TheDisplay = nullptr;


Display::Display(wxWindow *parent) : Panel(parent)
{
    TheDisplay = this;

    wxPanel::SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &Display::OnEventPaint, this);
    Bind(wxEVT_LEFT_DOWN, &Display::OnEventMouseDown, this);
    Bind(wxEVT_LEFT_UP, &Display::OnEventMouseUp, this);
    Bind(wxEVT_MOTION, &Display::OnEventMouseMove, this);
    Bind(wxEVT_MOUSEWHEEL, &Display::OnEventMouseWheel, this);
    Bind(wxEVT_RIGHT_DOWN, &Display::OnEventRightClick, this);
    Bind(wxEVT_LEAVE_WINDOW, &Display::OnEventLeaveWindow, this);
    Bind(wxEVT_ENTER_WINDOW, &Display::OnEventEnterWindow, this);
    Bind(wxEVT_BUTTON, &Display::OnEventButton, this);

    int w = 25;

    wxSize size{ w, w };

#define CREATE_BUTTON(btn, txt)                                         \
    btn = new wxButton(this, wxID_ANY, txt, wxDefaultPosition, size);   \
    buttons.push_back(btn);

    CREATE_BUTTON(btnHelp, "?")
    CREATE_BUTTON(btnLessX, "X-");
    CREATE_BUTTON(btnMoreX, "X+");
    CREATE_BUTTON(btnLessY, "Y-");
    CREATE_BUTTON(btnMoreY, "Y+");

    panel_errors = new PanelErrors(this);

//    new PanelTable(this);

//    PanelTable::self->Hide();

    Init();
}


void Display::ReInit()
{
//    for (auto elem : buttons)
//    {
//        elem->SetBackgroundColour(SET::GUI::color_background.Get());
//        elem->SetForegroundColour(SET::GUI::color_font.Get());
//        elem->SetOwnBackgroundColour(SET::GUI::color_background.Get());
//        elem->SetOwnForegroundColour(SET::GUI::color_font.Get());
//    }
}


Display::~Display()
{
    SAFE_DELETE(bitmap);
    SAFE_DELETE(TheGrid);
    SAFE_DELETE(panel_errors);
}


void Display::FullScreen(bool full)
{
    if (!IsShown())
    {
        return;
    }

    full_screen = full;

    Init();
}


void Display::Init()
{
    int width = full_screen ? MainWindow::WIDTH : MainWindow::WIDTH_DRAW;
    int height = full_screen ? MainWindow::HEIGHT : MainWindow::HEIGHT_DRAW;

    wxPanel::SetSize({ width, height });

    wxPanel::SetPosition({ 0, full_screen ? 0 : MainWindow::HEIGHT_HI });

    SAFE_DELETE(bitmap);

    bitmap = new wxBitmap(wxPanel::GetSize().x, wxPanel::GetSize().y);

    IGrid::Create();

    int w = btnHelp->GetSize().x;
    int d = 10;
    int x0 = wxPanel::GetSize().x - d - btnHelp->GetSize().x;
    int y0 = d;

    btnHelp->SetPosition({ x0, y0 });

    btnLessX->SetPosition({ x0 - 2 * (w + d), y0 });
    btnMoreX->SetPosition({ x0 - 1 * (w + d), y0 });

    btnLessY->SetPosition({ x0, y0 + 2 * (w + d) });
    btnMoreY->SetPosition({ x0, y0 + w + d });

    panel_errors->ReInit();

    wxPanel::Layout();

    GraphMeasure::CreateForEmulator(entities);

    ReInit();

    Refresh();
}


void Display::OnEventMouseDown(wxMouseEvent &event)
{
    pos_mouse_down = event.GetPosition();

    mouse_is_pressed = true;

    SetCursor(wxCursor(wxCURSOR_HAND));

    TheGrid->OnMouseDown();

    Refresh();
}


void Display::OnEventLeaveWindow(wxMouseEvent &event)
{
    if (mouse_is_pressed)
    {
        mouse_is_pressed = false;

        wxMouseEvent upEvent(wxEVT_LEFT_UP);
        upEvent.SetPosition(event.GetPosition());
        upEvent.SetEventObject(this);

        GetEventHandler()->ProcessEvent(upEvent);
    }

    TheAutoCursors->Ban();

    event.Skip();
}


void Display::OnEventEnterWindow(wxMouseEvent &event)
{
    TheAutoCursors->Allow();

    event.Skip();
}


void Display::OnEventMouseUp(wxMouseEvent &)
{
    mouse_is_pressed = false;

    SetCursor(wxCursor(wxCURSOR_ARROW));

    TheGrid->OnMouseUp();

    Refresh();
}


void Display::OnEventMouseMove(wxMouseEvent &event)
{
    wxPoint position = event.GetPosition();

    if (mouse_is_pressed)                            // Перемещение графика
    {
        wxPoint delta = position - pos_mouse_down;

        if (event.GetModifiers() == wxMOD_CONTROL)
        {
            TheGrid->MoveImageOn(delta);
        }
        else
        {
            TheGrid->MoveCenterOn(delta);
        }

        pos_mouse_down = position;

        if (!event.LeftIsDown())
        {

        }
    }
    else                                            // Отслеживание координат
    {
        TheGrid->SetNewMousePosition(position);
    }

    Refresh();
}


void Display::OnEventMouseWheel(wxMouseEvent &event)
{
    if (event.GetModifiers() == wxMOD_CONTROL)
    {
        TheGrid->ScaleGridOn(event.GetPosition(), event.GetWheelRotation());
    }
    else
    {
        TheGrid->RangeGridOnX(event.GetWheelRotation());
        TheGrid->RangeGridOnY(event.GetWheelRotation());
    }

    Refresh();
}


void Display::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnHelp->GetId())
    {
        wxMessageBox(_("Левая Кнопка Мыши - перемещение графика.\nКолёсико - масштаб графика.\n") +
            _("ЛКМ+Ctrl - перемещение сетки.\nКолёсико+Ctrl - масштаб сетки."), " ");
    }
    else if (id == btnLessX->GetId())
    {
        TheGrid->RangeGridOnX(-1);
    }
    else if (id == btnMoreX->GetId())
    {
        TheGrid->RangeGridOnX(+1);
    }
    else if (id == btnLessY->GetId())
    {
        TheGrid->RangeGridOnY(-1);
    }
    else if (id == btnMoreY->GetId())
    {
        TheGrid->RangeGridOnY(+1);
    }
}


void Display::BeginPaint()
{
    dc.SelectObject(*bitmap);
    gc = wxGraphicsContext::Create(dc);
    gc->SetAntialiasMode(wxANTIALIAS_NONE);
}


void Display::EndPaint()
{
    dc.SelectObject(wxNullBitmap);
}


void Display::OnEventPaint(wxPaintEvent &)
{
    BeginPaint();

    FillRectangle(0, 0, GetDrawingSize().x, GetDrawingSize().y, SET::GUI::color_background->Get());

    TheGrid->Draw(entities);

    EndPaint();

    wxPaintDC paint_dc(this);

    paint_dc.DrawBitmap(*bitmap, 0, 0);
}


void Point::Draw(int x, int y) const
{
    TheDisplay->gc->StrokeLine(x, y, x + 0.01, y);
}


void Line::Draw() const
{
    TheDisplay->gc->StrokeLine(x1, y1, x2, y2);
}


void Line::Draw(const wxColor &color) const
{
    TheDisplay->SetColorPen(color);
    TheDisplay->gc->StrokeLine(x1, y1, x2, y2);
}


void Rect::Fill(int x, int y, const wxColor &color) const
{
    TheDisplay->SetColorBrush(color);
    TheDisplay->gc->DrawRectangle(x, y, width, height);
}


void Rect::Draw(int x, int y, const wxColor &color) const
{
    TheDisplay->SetColorPen(color);
    Line(x, y, x + width, y).Draw();
    Line(x + width, y, x + width, y + height).Draw();
    Line(x, y + height, x + width, y + height).Draw();
    Line(x, y, x, y + height).Draw();
}


Text::Text(const wxString &_text) : text(_text)
{

}


void Text::SetFont()
{
    TheDisplay->gc->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), TheDisplay->color_pen);
}


void Text::Draw(int x, int y) const
{
    TheDisplay->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterLeft(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    TheDisplay->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    x -= (int)(width + 0.5);
    y -= (int)(height / 2.0 + 0.5);

    if (fillBackground)
    {
        TheDisplay->gc->SetPen(TheDisplay->color_brush);
        TheDisplay->gc->DrawRectangle(x, y, width, height);
        TheDisplay->gc->SetPen(TheDisplay->color_pen);
    }

    TheDisplay->gc->DrawText(text, x, y);
}


void Display::FillRectangle(int x, int y, int width, int height, const wxColor &_color)
{
    SetColorBrush(_color);
    gc->DrawRectangle(x, y, width, height);
}


void Text::DrawAboutCenterDown(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    TheDisplay->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    x -= (int)(width / 2.0 + 0.5);

    if (fillBackground)
    {
        TheDisplay->gc->SetPen(TheDisplay->color_brush);
        TheDisplay->gc->DrawRectangle(x, y, width, height);
        TheDisplay->gc->SetPen(TheDisplay->color_pen);
    }

    TheDisplay->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterUp(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    TheDisplay->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height);
    x -= (int)(width / 2);

    if (fillBackground)
    {
        TheDisplay->gc->SetPen(TheDisplay->color_brush);
        TheDisplay->gc->DrawRectangle(x, y, width, height);
        TheDisplay->gc->SetPen(TheDisplay->color_pen);
    }

    TheDisplay->gc->DrawText(text, x, y);
}


void Text::DrawAboutRightUp(int x, int y, bool fillBackground, bool frame) const
{
    double width, height, descent, externalLeading;
    TheDisplay->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height);

    if (fillBackground)
    {
        TheDisplay->gc->SetPen(TheDisplay->color_brush);
        TheDisplay->gc->DrawRectangle(x, y, width, height);
        TheDisplay->gc->SetPen(TheDisplay->color_pen);

        if (frame)
        {
            Rect((int)width, (int)height).Draw(x, y, TheDisplay->color_pen);
        }
    }

    TheDisplay->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterRigth(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    TheDisplay->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height / 2.0 + 0.5);

    if (fillBackground)
    {
        TheDisplay->gc->SetPen(TheDisplay->color_brush);
        TheDisplay->gc->DrawRectangle(x, y, width, height);
        TheDisplay->gc->SetPen(TheDisplay->color_pen);
    }

    TheDisplay->gc->DrawText(text, x, y);
}


void Spline::Draw(const std::vector<wxPoint> &points, bool smooth, bool draw_points) const
{
    if (smooth)
    {
        GraphicsSplineRenderer::DrawSplinePath(TheDisplay->gc, points, 1.0);
    }
    else
    {
        wxGraphicsPath path = TheDisplay->gc->CreatePath();

        path.MoveToPoint(points[0].x, points[0].y);

        for (uint i = 1; i < points.size(); i++)
        {
            path.AddLineToPoint(points[i].x, points[i].y);
        }

        TheDisplay->gc->StrokePath(path);
    }

    if (draw_points)
    {
        wxGraphicsPath path_circle = TheDisplay->gc->CreatePath();

        for (const auto &pt : points)
        {
            path_circle.AddCircle(pt.x, pt.y, SET::GUI::size_point->Get());
        }

        TheDisplay->gc->FillPath(path_circle);
    }
}


void Display::OnEventRightClick(wxMouseEvent &)
{
    MenuDisplay menu;

    PopupMenu(&menu);
}


void Display::SetColorBrush(const wxColor &_color)
{
    color_brush = _color;

    LoadColors();
}


void Display::SetColorPen(const wxColor &_color)
{
    color_pen = _color;

    LoadColors();
}


void Display::LoadColors()
{
    TheDisplay->gc->SetPen(color_pen);
    TheDisplay->gc->SetBrush(color_brush);
}


void Display::OnEventCnangeMeasuredElement()
{
}


wxSize Display::GetDrawingSize() const
{
    return wxPanel::GetClientSize();
}


wxSize Display::GetFullSize() const
{
    return wxPanel::GetSize();
}


