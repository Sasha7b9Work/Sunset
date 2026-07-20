// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define LOG_WRITE(...)   Log::Write(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) Log::Warning(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)   Log::Error(__FILE__, __LINE__, __VA_ARGS__)


namespace Log
{
    void Write(pchar file, int line, pchar format, ...);

    void Warning(pchar file, int line, pchar format, ...);

    void Error(pchar file, int line, pchar format, ...);
}
