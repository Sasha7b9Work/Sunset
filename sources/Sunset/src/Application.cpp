// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Utils/Configurator.h"
#include "Settings/Settings.h"
#include "MainWindow.h"
#include "Communicator/UART/UART.h"
#include "Communicator/SPI/SPI.h"
#include "Controls/AutoRebootDialog.h"
#include "SoftTests/SoftTests.h"
#include "IPPP/I_IPPP.h"
#include "IPPP/Real/Chips.h"
#include "IPPP/DeviceFactory.h"
#include "Windows/ConsoleRS232.h"
#include <cstdlib>


wxIMPLEMENT_APP(Application);


Application *TheApp = nullptr;


class NullLog : public wxLog
{
public:
    virtual void DoLogRecord(wxLogLevel,
        const wxString &,
        const wxLogRecordInfo &) override
    {
        // Ничего не делаем - просто игнорируем все логи
    }
};


#ifndef WIN32
// Функция-фильтр для логов
void glib_log_filter(const gchar *log_domain,
    GLogLevelFlags log_level,
    const gchar *message,
    gpointer user_data)
{
    // Игнорируем всё, что содержит "Gtk" или является предупреждением/критикой
    if (log_domain && (g_strcmp0(log_domain, "Gtk") == 0 ||
        g_strcmp0(log_domain, "Gtk-WARNING") == 0))
    {
        return; // Полностью подавляем
    }

    // Если нужно подавить только WARNING и CRITICAL, раскомментируйте следующее:
    // if (log_level & (G_LOG_LEVEL_WARNING | G_LOG_LEVEL_CRITICAL)) {
    //     return;
    // }

    // Для всех остальных сообщений — стандартный вывод
    g_log_default_handler(log_domain, log_level, message, user_data);
}
#endif


bool Application::OnInit()
{
    TheApp = this;

    // Попытка отключить предупреждения вида "Gtk-WARNING"
    wxLog::SetActiveTarget(new NullLog());

    // Попытка отключить предупреждения вида "Gtk-WARNING"
    // Устанавливаем переменные окружения для GTK (для Linux)
#ifndef __WXMSW__
    g_log_set_handler(NULL, (GLogLevelFlags)(G_LOG_LEVEL_WARNING | G_LOG_LEVEL_CRITICAL),
        [](const gchar *log_domain, GLogLevelFlags level,
            const gchar *message, gpointer user_data)
        {
            // Игнорируем предупреждения
        }, NULL);

    // Устанавливаем обработчик для доменов Gtk и GLib-GObject
    g_log_set_handler("Gtk",
        (GLogLevelFlags)(G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION),
        (GLogFunc)glib_log_filter,
        NULL);
    g_log_set_handler("GLib-GObject",
        (GLogLevelFlags)(G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION),
        (GLogFunc)glib_log_filter,
        NULL);

    setenv("G_MESSAGES_DEBUG", "0", 1);
    setenv("GTK_DEBUG", "0", 1);
    setenv("NO_AT_BRIDGE", "1", 1);
#endif

    std::locale::global(std::locale(""));  // Установка системной локали
    setlocale(LC_ALL, "");

    if (!wxApp::OnInit())
    {
        return false;
    }

#ifndef __WXMSW__
    // Включаем отображение иконок в контекстных меню GTK3
    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-menu-images", TRUE, NULL);
#endif

    SET::Init();

    Log::Init();

    Config::Init();

    SET::Load();

    // we use a PNG image in our HTML page
    wxImage::AddHandler(new wxPNGHandler);

    ConsoleRS232::Create();

    Chip::Init();

    auto device = DeviceFactory::CreateFromConfig();

    I_IPPP::SetInstance(std::move(device));

    // create and show the main application window
    new MainWindow(wxT("ИППП 4"));

    timer.SetOwner(this, timer.GetId());
    timer.Bind(wxEVT_TIMER, &Application::OnTimer, this, timer.GetId());

    if (!UART::IsAvailability())
    {
        wxString message = wxString::Format(wxT("Устройство UART %s не обнаружено."), UART_DEVICE);

        LOG_ERROR(message.c_str().AsChar());

        AutoRebootDialog dialog(TheMainWindow, message, 10, []
            {
                IGNORE_RESULT(std::system("shutdown -r now"));
            });

        dialog.ShowModal();
    }

    if (!SPI::IsAvailability())
    {
        wxString message = wxString::Format(wxT("Устройство SPI %s не обнаружено."), SPI_DEVICE);

        LOG_ERROR(message.c_str().AsChar());

        AutoRebootDialog dialog(TheMainWindow, message, 10, []
            {
                IGNORE_RESULT(std::system("shutdown -r now"));
            });

        dialog.ShowModal();
    }

    timer.Start(10);

    if (!SoftTests::RunAll())
    {
        wxMessageBox(wxString::Format(_("Во время выполнения тестов произошли ошибки.\n") +
            _("Дополнительная информация в файле %s."), Log::FileName().c_str().AsChar()), wxT("Ошибка"), wxOK | wxCENTRE | wxICON_ERROR);
    }

#ifdef WIN32

//    ComPort::Connect(PanelUpper::self->GetNumPort());

#endif

    if (SET::debug_mode->Get())
    {
        TheMainWindow->SetMode(ModeMainWindow::Debug);
    }

    if (!GF::IsBoardPCM())
    {
        TheMainWindow->Maximize(true);
    }

    TheMainWindow->Show();

    return true;
}


void Application::Disable()
{
    timer.Stop();

    SET::Save();
}


void Application::OnTimer(wxTimerEvent &)
{
    static std::mutex mutex;

    if (mutex.try_lock())
    {
        I_IPPP::GetInstance()->PeriodicTask();

        TheMainWindow->PeriodicTask();

        mutex.unlock();
    };
}


int Application::OnExit()
{
    Log::DeInit();

    Config::DeInit();

    return wxApp::OnExit();
}


void Application::OnReceiveUART(uint8)
{

}


void Application::OnGovernor(int)
{

}


void Application::OnButtonStart(bool /*press*/)
{

}


void Application::OnButtonStop(bool /*press*/)
{

}


void Application::ReInit()
{

}
