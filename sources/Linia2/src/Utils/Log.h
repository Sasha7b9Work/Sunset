// 2023/08/31 13:22:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define LOG_WRITE(...)          Log::Write(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_WRITE_TRACE(...)    Log::WriteTrace(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#define LOG_ERROR(...)          Log::Error(__FILE__, __LINE__, __VA_ARGS__)
//#define LOG_ERROR_TRACE(...)    Log::ErrorTrace(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)


namespace Log
{
    void Init();

    wxString FileName();

    void DeInit();

    void FromDevice(char *);

    void ToDevice(char *);

    void Write(pchar file, int line, pchar format, ...);

    void Error(pchar file, int line, pchar format, ...);

    void WriteTrace(pchar file, int line, pchar function, pchar format, ...);

    void ErrorTrace(pchar file, int line, pchar function, pchar format, ...);

    // Вывод сообщения от stm32
    void LogMCU(pchar type, pchar message);
}
