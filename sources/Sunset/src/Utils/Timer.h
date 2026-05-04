// 2022/05/24 15:12:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <chrono>


namespace Timer
{
    int64 CurrentTimeMS();
    uint CurrentTimeSec();

    void PauseOnMS(uint);
}


// Структура для отсчёта времени
struct TimeMeterMS
{
    TimeMeterMS()
    {
        Reset();
    }

    // Установить момент отсчёта
    void Reset();

    float ElapsedMS() const;

private:

    std::chrono::high_resolution_clock::time_point time_reset;      // От этого времени отсчитывается ElapsedTime()
};


struct TimeMeterNS
{
    TimeMeterNS()
    {
        Reset();
    }

    void Reset();

    float ElapsedNS() const;

private:

    std::chrono::high_resolution_clock::time_point time_reset;
};