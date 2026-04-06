// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Log.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include "Device/PCM/SCPI.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


namespace Log
{
    static int counter = 0;

    static void WriteLine(char *);
}


void Log::Write(pchar file, int line, pchar format, ...) //-V2560
{
    char message[1024];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    char full_message[2048];

    std::sprintf(full_message, ":LOG:%3d : %s:%3d : %s", counter++, SU::LeaveTheLastOnes(file, 27), line, message);

    WriteLine(full_message);
}


void Log::Warning(pchar file, int line, pchar format, ...) //-V2560
{
    char message[1024];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    String text_string(":WARNING:%3d : %s:%3d : %s", counter++, SU::LeaveTheLastOnes(file, 27), line, message);

    WriteLine(text_string.c_str());
}


void Log::Error(pchar file, int line, pchar format, ...) //-V2560
{
    char message[1024];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    String text_string(":ERROR:%3d : %s:%3d : %s", counter++, SU::LeaveTheLastOnes(file, 27), line, message);

    WriteLine(text_string.c_str());
}


void Log::WriteLine(char *line)
{
    // Устанавливаем 7-й бит - приёмное устройство не будет приводить символы к верхнему регистру

    char *pointer = line;

    while (*pointer)
    {
        (*pointer) |= 0x80;
        pointer++;
    }

    PCM::SCPI::Send(line);
}
