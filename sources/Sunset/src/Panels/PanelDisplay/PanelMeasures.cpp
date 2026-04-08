// 2022/10/28 23:17:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDisplay/PanelMeasures.h"
#include "MainWindow.h"
#include "MainWindow.h"
#include "Panels/PanelDisplay/Graphics/GraphMeasure.h"
#include "Panels/PanelDisplay/MenuDisplay.h"
#include "Panels/PanelDisplay/Graphics/Splines.h"
#include "Settings/Settings.h"
#include "Utils/Timer.h"
#include "Panels/PanelDisplay/Graphics/AutoCursors.h"


PanelDisplay *ThePanelMeasures = nullptr;


PanelDisplay::PanelDisplay(wxWindow *parent) : Panel(parent, "Измерение")
{
    ThePanelMeasures = this;

    wxPanel::SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &PanelDisplay::OnEventPaint, this);
    Bind(wxEVT_LEFT_DOWN, &PanelDisplay::OnEventMouseDown, this);
    Bind(wxEVT_LEFT_UP, &PanelDisplay::OnEventMouseUp, this);
    Bind(wxEVT_MOTION, &PanelDisplay::OnEventMouseMove, this);
    Bind(wxEVT_MOUSEWHEEL, &PanelDisplay::OnEventMouseWheel, this);
    Bind(wxEVT_RIGHT_DOWN, &PanelDisplay::OnEventRightClick, this);
    Bind(wxEVT_LEAVE_WINDOW, &PanelDisplay::OnEventLeaveWindow, this);
    Bind(wxEVT_ENTER_WINDOW, &PanelDisplay::OnEventEnterWindow, this);
    Bind(wxEVT_BUTTON, &PanelDisplay::OnEventButton, this);

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

//    Init();
}


PanelDisplay::~PanelDisplay()
{
    SAFE_DELETE(bitmap);
    SAFE_DELETE(TheGrid);
    SAFE_DELETE(panel_errors);
}


void PanelDisplay::FullScreen(bool full)
{
    if (!IsShown())
    {
        return;
    }

    full_screen = full;
}


void PanelDisplay::CallbackOnEventSize()
{
    const wxSize size = GetParent()->GetSize();

    if (size == GetSize())
    {
        return;
    }

    SetMinSize(size);
    SetSize(size);

    SAFE_DELETE(bitmap);

    bitmap = new wxBitmap(size.x, size.y);

    IGrid::Create();

    int w = btnHelp->GetSize().x;
    int d = 10;
    int x0 = size.x - d - btnHelp->GetSize().x;
    int y0 = d;

    btnHelp->SetPosition({ x0, y0 });

    btnLessX->SetPosition({ x0 - 2 * (w + d), y0 });
    btnMoreX->SetPosition({ x0 - 1 * (w + d), y0 });

    btnLessY->SetPosition({ x0, y0 + 2 * (w + d) });
    btnMoreY->SetPosition({ x0, y0 + w + d });

    panel_errors->ReInit();

    Layout();

    GraphMeasure::CreateForEmulator(entities);
}


void PanelDisplay::OnEventMouseDown(wxMouseEvent &event)
{
    pos_mouse_down = event.GetPosition();

    mouse_is_pressed = true;

    SetCursor(wxCursor(wxCURSOR_HAND));

    TheGrid->OnMouseDown();

    Refresh();
}


void PanelDisplay::OnEventLeaveWindow(wxMouseEvent &event)
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


void PanelDisplay::OnEventEnterWindow(wxMouseEvent &event)
{
    TheAutoCursors->Allow();

    event.Skip();
}


void PanelDisplay::OnEventMouseUp(wxMouseEvent &)
{
    mouse_is_pressed = false;

    SetCursor(wxCursor(wxCURSOR_ARROW));

    TheGrid->OnMouseUp();

    Refresh();
}


void PanelDisplay::OnEventMouseMove(wxMouseEvent &event)
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


void PanelDisplay::OnEventMouseWheel(wxMouseEvent &event)
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


void PanelDisplay::OnEventButton(wxCommandEvent &event)
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


void PanelDisplay::BeginPaint()
{
    dc.SelectObject(*bitmap);
    gc = wxGraphicsContext::Create(dc);
    gc->SetAntialiasMode(wxANTIALIAS_NONE);
}


void PanelDisplay::EndPaint()
{
    dc.SelectObject(wxNullBitmap);
}


void PanelDisplay::OnEventPaint(wxPaintEvent &)
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
    ThePanelMeasures->gc->StrokeLine(x, y, x + 0.01, y);
}


void Line::Draw() const
{
    ThePanelMeasures->gc->StrokeLine(x1, y1, x2, y2);
}


void Line::Draw(const wxColor &color) const
{
    ThePanelMeasures->SetColorPen(color);
    ThePanelMeasures->gc->StrokeLine(x1, y1, x2, y2);
}


void Rect::Fill(int x, int y, const wxColor &color) const
{
    ThePanelMeasures->SetColorBrush(color);
    ThePanelMeasures->gc->DrawRectangle(x, y, width, height);
}


void Rect::Draw(int x, int y, const wxColor &color) const
{
    ThePanelMeasures->SetColorPen(color);
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
    ThePanelMeasures->gc->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), ThePanelMeasures->color_pen);
}


void Text::Draw(int x, int y) const
{
    ThePanelMeasures->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterLeft(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    ThePanelMeasures->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    x -= (int)(width + 0.5);
    y -= (int)(height / 2.0 + 0.5);

    if (fillBackground)
    {
        ThePanelMeasures->gc->SetPen(ThePanelMeasures->color_brush);
        ThePanelMeasures->gc->DrawRectangle(x, y, width, height);
        ThePanelMeasures->gc->SetPen(ThePanelMeasures->color_pen);
    }

    ThePanelMeasures->gc->DrawText(text, x, y);
}


void PanelDisplay::FillRectangle(int x, int y, int width, int height, const wxColor &_color)
{
    SetColorBrush(_color);
    gc->DrawRectangle(x, y, width, height);
}


void Text::DrawAboutCenterDown(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    ThePanelMeasures->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    x -= (int)(width / 2.0 + 0.5);

    if (fillBackground)
    {
        ThePanelMeasures->gc->SetPen(ThePanelMeasures->color_brush);
        ThePanelMeasures->gc->DrawRectangle(x, y, width, height);
        ThePanelMeasures->gc->SetPen(ThePanelMeasures->color_pen);
    }

    ThePanelMeasures->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterUp(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    ThePanelMeasures->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height);
    x -= (int)(width / 2);

    if (fillBackground)
    {
        ThePanelMeasures->gc->SetPen(ThePanelMeasures->color_brush);
        ThePanelMeasures->gc->DrawRectangle(x, y, width, height);
        ThePanelMeasures->gc->SetPen(ThePanelMeasures->color_pen);
    }

    ThePanelMeasures->gc->DrawText(text, x, y);
}


void Text::DrawAboutRightUp(int x, int y, bool fillBackground, bool frame) const
{
    double width, height, descent, externalLeading;
    ThePanelMeasures->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height);

    if (fillBackground)
    {
        ThePanelMeasures->gc->SetPen(ThePanelMeasures->color_brush);
        ThePanelMeasures->gc->DrawRectangle(x, y, width, height);
        ThePanelMeasures->gc->SetPen(ThePanelMeasures->color_pen);

        if (frame)
        {
            Rect((int)width, (int)height).Draw(x, y, ThePanelMeasures->color_pen);
        }
    }

    ThePanelMeasures->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterRigth(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    ThePanelMeasures->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height / 2.0 + 0.5);

    if (fillBackground)
    {
        ThePanelMeasures->gc->SetPen(ThePanelMeasures->color_brush);
        ThePanelMeasures->gc->DrawRectangle(x, y, width, height);
        ThePanelMeasures->gc->SetPen(ThePanelMeasures->color_pen);
    }

    ThePanelMeasures->gc->DrawText(text, x, y);
}


void Spline::Draw(const std::vector<wxPoint> &points, bool smooth, bool draw_points) const
{
    if (smooth)
    {
        GraphicsSplineRenderer::DrawSplinePath(ThePanelMeasures->gc, points, 1.0);
    }
    else
    {
        wxGraphicsPath path = ThePanelMeasures->gc->CreatePath();

        path.MoveToPoint(points[0].x, points[0].y);

        for (uint i = 1; i < points.size(); i++)
        {
            path.AddLineToPoint(points[i].x, points[i].y);
        }

        ThePanelMeasures->gc->StrokePath(path);
    }

    if (draw_points)
    {
        wxGraphicsPath path_circle = ThePanelMeasures->gc->CreatePath();

        for (const auto &pt : points)
        {
            path_circle.AddCircle(pt.x, pt.y, SET::GUI::size_point->Get());
        }

        ThePanelMeasures->gc->FillPath(path_circle);
    }
}


void PanelDisplay::OnEventRightClick(wxMouseEvent &)
{
    MenuDisplay menu;

    PopupMenu(&menu);
}


void PanelDisplay::SetColorBrush(const wxColor &_color)
{
    color_brush = _color;

    LoadColors();
}


void PanelDisplay::SetColorPen(const wxColor &_color)
{
    color_pen = _color;

    LoadColors();
}


void PanelDisplay::LoadColors()
{
    ThePanelMeasures->gc->SetPen(color_pen);
    ThePanelMeasures->gc->SetBrush(color_brush);
}


void PanelDisplay::OnEventCnangeMeasuredElement()
{
}


wxSize PanelDisplay::GetDrawingSize() const
{
    return wxPanel::GetClientSize();
}


wxSize PanelDisplay::GetFullSize() const
{
    return wxPanel::GetSize();
}
