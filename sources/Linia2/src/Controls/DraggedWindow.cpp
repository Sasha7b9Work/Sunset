// 2026/3/16 21:10:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/DraggedWindow.h"
#include "Settings/Settings.h"
#include "MainWindow.h"
#include "Display/Graphics/AutoCursors.h"


DraggedWindow::DraggedWindow(const wxString &_title, const wxSize &_size)
    : wxFrame(TheMainWindow, wxID_ANY, "WindowCalculation",
        { TheMainWindow->GetSize().x / 2, TheMainWindow->GetSize().y / 2 }, _size,
        wxFRAME_FLOAT_ON_PARENT | wxBORDER_SIMPLE | wxSTAY_ON_TOP),
    title(_title)
{
    TheAutoCursors->Ban();

    main_panel = new wxPanel(this, wxID_ANY, { 0, 0 }, _size, wxNO_BORDER | wxEXPAND | wxSTAY_ON_TOP);

#ifdef WIN32
    SetupDragging(main_panel);
#else
    SetupDragging(main_panel);
#endif

    Layout();
    Fit();

    main_panel->Bind(wxEVT_PAINT, &DraggedWindow::OnEventPaint, this);

    Bind(wxEVT_CLOSE_WINDOW, &DraggedWindow::OnEventClose, this);
    Bind(wxEVT_LEAVE_WINDOW, &DraggedWindow::OnEventMouseLeave, this);

    SetSize(_size);

    position = new ValuePoint(_title + "_pos", { 100, 100 }),
    SetPosition();
}


DraggedWindow::~DraggedWindow()
{
    TheAutoCursors->Allow();
}


void DraggedWindow::SetPosition()
{
    position->Load();
    wxFrame::SetPosition(position->Get());
}


void DraggedWindow::CreateTitleBar()
{

}


void DraggedWindow::SetSize(const wxSize &_size)
{
    wxSize new_size = _size;
    new_size.y += titleHeight;

    wxFrame::SetClientSize(new_size);

    main_panel->SetSize(_size);
    main_panel->SetPosition( { 0, titleHeight });

    main_panel->Layout();
    main_panel->Fit();
}


void DraggedWindow::SetupDragging(wxWindow *window)
{
    window->Bind(wxEVT_LEFT_DOWN, &DraggedWindow::OnEventMouseLeftDown, this);
    window->Bind(wxEVT_LEFT_UP, &DraggedWindow::OnEventMouseLeftUp, this);
    window->Bind(wxEVT_MOTION, &DraggedWindow::OnEventMouseMotion, this);
}

void DraggedWindow::OnEventMouseLeftDown(wxMouseEvent &event)
{
    if (!dragging)
    {
        // Обработка захывата мыши

        wxWindow *source = (wxWindow *)event.GetEventObject();
        if (source && !source->HasCapture())
        {
            source->CaptureMouse();
        }

        dragging = true;
        dragStart = wxGetMousePosition();

    }

    event.Skip();
}


void DraggedWindow::OnEventClose(wxCloseEvent &event)
{
    position->Set(wxFrame::GetPosition());

    Destroy();

    event.Skip();
}


void DraggedWindow::OnEventMouseLeftUp(wxMouseEvent &event)
{
    {
        // Обработка перемещения мышью

        if (dragging)
        {
            wxWindow *source = (wxWindow *)event.GetEventObject();
            if (source && source->HasCapture())
            {
                source->ReleaseMouse();
            }

            dragging = false;
        }
    }

    {
        // Обработка нажатия кнопки мыш

        wxPoint pos = event.GetPosition();

        if (closeButtonRect.Contains(pos))
        {
            position->Set(wxFrame::GetPosition());
            Close();
            return;
        }
    }

    event.Skip();
}


void DraggedWindow::OnEventMouseLeave(wxMouseEvent &event)
{
    mouseInCloseButton = false;
    Refresh();

    event.Skip();
}


void DraggedWindow::OnEventMouseMotion(wxMouseEvent &event)
{
    wxWindow *source = (wxWindow *)event.GetEventObject();

    {
        // Обработка подсветки кнопки ЭАКРЫТЬ

        wxPoint pos = event.GetPosition();

        // Проверяем, находится ли мышь над кнопкой закрытия
        bool wasInButton = mouseInCloseButton;
        mouseInCloseButton = closeButtonRect.Contains(pos);

        // Если состояние изменилось, перерисовываем
        if (wasInButton != mouseInCloseButton)
        {
            RefreshRect(closeButtonRect);  // Перерисовываем только область кнопки
        }
    }

    {
        // Обработка перемещения окна

        if (dragging && event.Dragging() && source && source->HasCapture())
        {
            wxPoint currentPos = wxGetMousePosition();
            wxPoint delta = currentPos - dragStart;
            wxPoint newPos = GetPosition() + delta;
            Move(newPos);
            dragStart = currentPos;
        }
    }

    event.Skip();
}


bool DraggedWindow::Show(bool show)
{
    bool result = wxFrame::Show(show);

    TheMainWindow->HideSystemPanel();

    return result;
}


void DraggedWindow::OnEventPaint(wxPaintEvent &)
{
    wxPaintDC dc(main_panel);

    int width, height;
    GetClientSize(&width, &height);

    // Градиентная заливка заголовка
    wxColour startColor(70, 130, 180);  // Steel blue
    wxColour endColor(100, 149, 237);   // Cornflower blue

    for (int y = 0; y < titleHeight; y++)
    {
        float ratio = (float)y / (float)titleHeight;
        wxColour color(
            (uint8)(startColor.Red() * (1 - ratio) + endColor.Red() * ratio),
            (uint8)(startColor.Green() * (1 - ratio) + endColor.Green() * ratio),
            (uint8)(startColor.Blue() * (1.0f - ratio) + endColor.Blue() * ratio)
        );
        dc.SetPen(wxPen(color));
        dc.SetBrush(wxBrush(color));
        dc.DrawLine(0, y, width, y);
    }

    // Рисуем рамку вокруг заголовка
    dc.SetPen(wxPen(wxColour(50, 50, 50)));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(0, 0, width, titleHeight);

    // Рисуем текст заголовка
    dc.SetTextForeground(*wxWHITE);
    dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    // Отступаем от левого края 10 пикселей
    dc.DrawText(title, 10, (titleHeight - dc.GetCharHeight()) / 2);

    // Рисуем кнопку закрытия (квадратик 20x20 справа)
    int buttonSize = 16;
    int buttonX = width - buttonSize - 5;  // Отступ 5 пикселей от края
    int buttonY = (titleHeight - buttonSize) / 2;

    closeButtonRect = wxRect(buttonX, buttonY, buttonSize, buttonSize);

    // Рисуем фон кнопки
    if (mouseInCloseButton)
    {
        dc.SetBrush(wxBrush(wxColour(220, 80, 80)));  // Красный при наведении
        dc.SetPen(wxPen(wxColour(200, 60, 60)));
    }
    else
    {
        dc.SetBrush(wxBrush(wxColour(150, 150, 150)));  // Серый обычно
        dc.SetPen(wxPen(wxColour(120, 120, 120)));
    }
    dc.DrawRectangle(closeButtonRect);

    // Рисуем крестик
    dc.SetPen(wxPen(*wxWHITE, 2));
    int margin = 5;  // Отступ от краев кнопки для крестика

    // Линии крестика
    dc.DrawLine(buttonX + margin, buttonY + margin,
        buttonX + buttonSize - margin, buttonY + buttonSize - margin);
    dc.DrawLine(buttonX + buttonSize - margin, buttonY + margin,
        buttonX + margin, buttonY + buttonSize - margin);
}


DraggedDialog::DraggedDialog(const wxString &_title, const wxSize &_size) : DraggedWindow(_title, _size)
{
    SetWindowStyleFlag(wxFRAME_FLOAT_ON_PARENT | wxBORDER_SIMPLE);

    m_parent = GetParent();
    m_modalActive = false;
    m_modalResult = wxID_CANCEL;

    Bind(wxEVT_CLOSE_WINDOW, &DraggedDialog::OnEventClose, this);

    Layout();
    Fit();
}


DraggedDialog::~DraggedDialog()
{
}


int DraggedDialog::ShowModal()
{
    TheAutoCursors->Ban();

    m_modalActive = true;
    m_modalResult = wxID_CANCEL;

    // Блокируем родителя
    if (m_parent)
    {
        m_parent->Disable();
    }

    // Показываем окно с XFCE фиксом
    ShowWithXFCEFix();

    // Простой цикл ожидания
    while (m_modalActive)
    {
        wxYield();          // Обрабатываем события
        wxMilliSleep(10);   // Небольшая задержка чтобы не нагружать CPU
    }

    TheAutoCursors->Allow();

    position->Set(wxFrame::GetPosition());

    return m_modalResult;
}


void DraggedDialog::ShowWithXFCEFix()
{
    Show();
    Raise();
    SetFocus();

#ifdef __WXGTK__
    wxYield();  // Даем время GTK/XFCE

    GtkWidget *widget = GTK_WIDGET(GetHandle());
    if (widget && gtk_widget_get_window(widget))
    {
        GdkWindow *gdkWindow = gtk_widget_get_window(widget);
        gdk_window_set_keep_above(gdkWindow, TRUE);
        gdk_window_raise(gdkWindow);
    }
#endif
}


void DraggedDialog::OnEventClose(wxCloseEvent &)
{
    CloseModal();
}


void DraggedDialog::OnOK(wxCommandEvent &)
{
    m_modalResult = wxID_OK;
    CloseModal();
}


void DraggedDialog::OnCancel(wxCommandEvent &)
{
    m_modalResult = wxID_CANCEL;
    CloseModal();
}


void DraggedDialog::CloseModal()
{
    m_modalActive = false;
    Hide();

    // Разблокируем родителя
    if (m_parent)
    {
        m_parent->Enable();
        m_parent->Raise();  // Поднимаем родительское окно
    }
}


bool DraggedDialog::Show(bool show)
{
    return DraggedWindow::Show(show);
}
