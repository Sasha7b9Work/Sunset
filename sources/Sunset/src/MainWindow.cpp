// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Utils/Configurator.h"
#include "Settings/Settings.h"
#include "Utils/Configurator.h"
#include "Windows/ConsoleRS232.h"
#include "MainWindow.h"
#include "MainPanel/PanelMeasures/PanelMeasures.h"
#include "MainPanel/PanelTests/PanelTests.h"
#include "MainPanel/PanelDebug/PanelDebug.h"
#include "MainPanel/PanelMeasures/Graphics/AutoCursors.h"
#include "MainPanel/PanelArchive/PanelArchive.h"
#include "MainPanel/PanelReports/PanelReports.h"
#include "MainPanel/PanelSettings/PanelSettings.h"
#include "MainPanel/PanelTables/PanelTables.h"
#include "Controls/Dialog.h"
#include "IPPP/Tests/Tests.h"
#include "Controls/Notebook.h"
#pragma warning(push, 0)
#include <wx/sizer.h>
#include <wx/statline.h>
#pragma warning(pop)

MainWindow *TheMainWindow = nullptr;


ModeMainWindow::E ModeMainWindow::current = ModeMainWindow::Standard;


MainWindow::MainWindow(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{
    TheMainWindow = this;

#ifdef WIN32
    SetIcon(wxICON(MAIN_ICON));
#endif

    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_MAXIMIZE, &MainWindow::OnEventMaximize, this);
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnEventCloseWindow, this);

    TuneFont();

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    board = new Notebook(this);
    board->AddPanel(new PanelDebug(board, ThePanelDebug));
    board->AddPanel(new PanelMeasures(board, ThePanelMeasures));
    board->AddPanel(new PanelTests(board, ThePanelTests));
    board->AddPanel(new PanelTables(board, ThePanelTables));
    board->AddPanel(new PanelReports(board, ThePanelReports));
    board->AddPanel(new PanelArchive(board, ThePanelArchive));
    board->AddPanel(new PanelSettings(board, ThePanelSettings));

    sizer->Add(board, 1, wxEXPAND);
    SetSizer(sizer);

//    wxYield();
//    board->SetCurrentPanelIndex(2);
//    wxYield();
//    board->SetCurrentPanelIndex(1);
//    wxYield();
    board->SetCurrentPanelIndex(0);
//    wxYield();

    SetPosition();

    Bind(wxEVT_CHAR_HOOK, &MainWindow::OnEventKeyHook, this);

    {
        if (GF::IsBoardPCM())
        {
            SetSize({ WIDTH, HEIGHT });

            ShowFullScreen(true);
        }
        else
        {
            SetClientSize({ WIDTH, HEIGHT });

#ifndef WIN32
            SetWindowStyle(GetWindowStyle() & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
#endif
        }
    }

    new AutoCursors();

    SetMode(ModeMainWindow::Standard);
}


void MainWindow::PeriodicTask()
{

}


void MainWindow::OnEventKeyHook(wxKeyEvent &event)
{
    if (event.GetKeyCode() == 'K')
    {
        if (event.ControlDown())
        {
            ConsoleRS232::self->Show(!ConsoleRS232::self->IsShown());
        }
    }

    event.Skip();
}


void MainWindow::OnEventMaximize(wxMaximizeEvent &event)
{
    // Без этого первая максимизация окна приложения не отрабатывает на элементах сайзера

    Layout();
    GetSizer()->Layout();
    Refresh();

    event.Skip();
}


void MainWindow::TuneFont()
{
    wxFont font = GetFont();

    //------------------------------------------------------------

    wxFontFamily family = wxFONTFAMILY_DEFAULT;

    Config::ReadFontParameter("font_family", family);

    font.SetFamily(family);

    //------------------------------------------------------------

    wxString face_name = "Segoe UI";

    Config::ReadString("font_face_name", face_name);

    font.SetFaceName(face_name);

    //------------------------------------------------------------

    wxFontStyle style = wxFONTSTYLE_NORMAL;

    Config::ReadFontParameter("font_style", style);

    font.SetStyle(style);

    //------------------------------------------------------------

    int point_size = 9;

    Config::ReadInt("font_point_size", point_size);

    font.SetPointSize(point_size);

    //------------------------------------------------------------

    wxPoint pixel_size{ font.GetPixelSize().x, font.GetPixelSize().y };

    Config::Read("font_pixel_size_x", pixel_size.x);
    Config::Read("font_pixel_size_y", pixel_size.y);

    font.SetPixelSize({ pixel_size.x, pixel_size.y });

    //------------------------------------------------------------

    font.SetUnderlined(false);

    font.SetStrikethrough(false);

    //------------------------------------------------------------

    wxFontWeight weigth = wxFONTWEIGHT_NORMAL;

    Config::ReadFontParameter("font_weigth", weigth);

    font.SetWeight(weigth);

    //------------------------------------------------------------

    wxWindow::SetFont(font);
}


void MainWindow::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    LOG_WRITE("Frame::OnQuit()");

    TheMainWindow = nullptr;

    Close(true);
}


void MainWindow::OnEventCloseWindow(wxCloseEvent &event)
{
    Test::Save("example.tst");

    SET::GUI::current_panel->Set(board->GetCurrentPanelIndex());

    if (ConsoleRS232::self)
    {
        SET::GUI::pos_console->Set(ConsoleRS232::self->GetPosition());

        wxSize size = ConsoleRS232::self->GetSize();
        SET::GUI::size_console->Set({ size.x, size.y });

        SET::GUI::maximized_console->Set(ConsoleRS232::self->IsMaximized());

        ConsoleRS232::self->Destroy();
    }

    position.Set(wxWindow::GetPosition());

    TheMainWindow = nullptr;

    TheApp->Disable();

    event.Skip();
}


void MainWindow::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxBoxSizer *topsizer;
    Dialog dlg(wxString(L("About")));

    topsizer = new wxBoxSizer(wxVERTICAL);

#if wxUSE_STATLINE
    topsizer->Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
#endif // wxUSE_STATLINE

    wxButton *bu1 = new wxButton(&dlg, wxID_OK, L("OK"));
    bu1->SetDefault();

    topsizer->Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);

    dlg.SetSizer(topsizer);
    topsizer->Fit(&dlg);

    dlg.ShowModal();
}


void MainWindow::SetMode(ModeMainWindow::E mode)
{
    ModeMainWindow::current = mode;

    if (ThePanelMeasures)
    {
        ThePanelMeasures->Show(mode == ModeMainWindow::Standard || mode == ModeMainWindow::FullGraph);

        ThePanelMeasures->FullScreen(mode == ModeMainWindow::FullGraph);
    }

    wxFrame::Layout();
}


void MainWindow::HideSystemPanel()
{
    Raise();

#ifdef __WXGTK__
    GtkWidget *widget = GetHandle(); // В wxGTK GetHandle() возвращает GtkWidget*
    if (widget && gtk_widget_get_window(widget))
    {
        GdkWindow *gdk_window = gtk_widget_get_window(widget);
        // Делаем окно "выше всех" средствами GDK
        gdk_window_set_keep_above(gdk_window, TRUE);
        // Принудительно поднимаем
        gdk_window_raise(gdk_window);
    }
#endif
}


void MainWindow::SetPosition()
{
    position.Load();

    wxWindow::SetPosition(position.Get());
}
