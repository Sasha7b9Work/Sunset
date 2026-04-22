// 2023/08/31 13:22:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Log.h"
#include "Utils/StringUtils.h"
#include "Windows/ConsoleRS232.h"
#pragma warning(push, 0)
#include <wx/textfile.h>
#include <wx/filename.h>
#pragma warning(pop)


namespace Log
{
    static std::mutex mutex;

    static int counter = 1;

    static wxString file_name;

    static wxTextFile log_file;

    static wxString GetTime();

    // Ограничивает размер файла лога чтобы не тормозил работу приложения
    static void CutSize();

    static void WriteLine(pchar);
}


void Log::Init()
{
    mutex.lock();

    {
        wxFileName fname{ wxGetCwd() + "/Linia.log" };

        fname.Normalize(wxPATH_NORM_ABSOLUTE);

        file_name = fname.GetFullPath();

        if (wxFile::Exists(file_name))
        {
            log_file.Open(file_name);
        }
        else
        {
            log_file.Create(file_name);
        }

        CutSize();
    }

    WriteLine("");

    mutex.unlock();

    LOG_WRITE("Start application %s", wxDateTime::Now().Format("%Y-%m-%d").c_str().AsChar());
}


wxString Log::FileName()
{
    return file_name;
}


void Log::DeInit()
{
    LOG_WRITE("Exit application %s", wxDateTime::Now().Format("%Y-%m-%d").c_str().AsChar());

    mutex.lock();

    CutSize();

    mutex.unlock();
}


void Log::WriteLine(pchar line)
{
    log_file.AddLine(line);

    log_file.Write();

    std::cerr << line << std::endl;
}


void Log::FromDevice(char *line)
{
    mutex.lock();

    {
        WriteLine((wxString(REVERSE_PROMT) + line).c_str().AsChar());
    }

    mutex.unlock();
}


void Log::ToDevice(char *line)
{
    mutex.lock();

    {
        WriteLine((wxString(DIRECT_PROMT) + line).c_str().AsChar());
    }

    mutex.unlock();
}


void Log::Error(pchar file, int line, pchar format, ...)
{
    mutex.lock();

    {
        char message[1024];

        std::va_list args;
        va_start(args, format);
        std::vsprintf(message, format, args);
        va_end(args);

        String text_string(wxString::Format("!!! ERROR !!! %3d : %s:%3d : %s : %s", counter++, SU::LeaveTheLastOnes(file, 17), line, GetTime().c_str(), message).c_str());

        ConsoleRS232::self->AddLine(text_string.c_str());

        WriteLine(text_string.c_str());
    }

    mutex.unlock();
}


void Log::ErrorTrace(pchar file, int line, pchar function, pchar format, ...)
{
    mutex.lock();

    {
        char message[1024];

        std::va_list args;
        va_start(args, format);
        std::vsprintf(message, format, args);
        va_end(args);

        String text_string(wxString::Format("!!! ERROR !!! %3d : %s:%3d : %s() : %s : %s", counter++, SU::LeaveTheLastOnes(file, 17), line, function, GetTime().c_str(), message).c_str());

        ConsoleRS232::self->AddLine(text_string.c_str());

        WriteLine(text_string.c_str());
    }

    mutex.unlock();
}


void Log::Write(pchar file, int line, pchar format, ...)
{
    mutex.lock();

    {
        char message[1024];

        std::va_list args;
        va_start(args, format);
        std::vsprintf(message, format, args);
        va_end(args);

        String text_string(wxString::Format("Log %3d : %s:%3d : %s : %s", counter++, SU::LeaveTheLastOnes(file, 27), line, GetTime().c_str(), message).c_str());

        if (ConsoleRS232::self)
        {
            ConsoleRS232::self->AddLine(text_string.c_str());
        }

        WriteLine(text_string.c_str());
    }

    mutex.unlock();
}


void Log::LogMCU(pchar type, pchar message)
{
    char buffer[1024];

    std::sprintf(buffer, "%s %s", type, message);

    WriteLine(buffer);
}


void Log::WriteTrace(pchar file, int line, pchar function, pchar format, ...)
{
    mutex.lock();

    {
        char message[1024];

        std::va_list args;
        va_start(args, format);
        std::vsprintf(message, format, args);
        va_end(args);

        String text_string(wxString::Format("Log %3d : %s:%3d : %s() : %s : %s", counter++, SU::LeaveTheLastOnes(file, 27), line, function, GetTime().c_str(), message).c_str());

        if (ConsoleRS232::self)
        {
            ConsoleRS232::self->AddLine(text_string.c_str());
        }

        WriteLine(text_string.c_str());
    }

    mutex.unlock();
}


pchar SU::LeaveTheLastOnes(pchar in, int number)
{
    int size = (int)std::strlen(in);

    if (size >= number)
    {
        return in + size - number;
    }

    static char buffer[1024];

    char *pointer = buffer;

    int num_spaces = number - size;

    for (int i = 0; i < num_spaces; i++)
    {
        *pointer++ = ' ';
    }

    *pointer = '\0';

    std::strcat(buffer, in);

    return buffer;
}


wxString Log::GetTime()
{
    wxDateTime now = wxDateTime::Now();

    return wxString::Format("%02d:%02d:%02d",
        now.GetHour(),
        now.GetMinute(),
        now.GetSecond());
}


void Log::CutSize()
{
    if (log_file.GetLineCount() > 10000)
    {
        while (log_file.GetLineCount() > 9500)
        {
            log_file.RemoveLine(0);
        }
    }
}
