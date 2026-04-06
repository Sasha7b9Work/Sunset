// 2022/05/24 15:13:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Timer.h"


void TimeMeterMS::Reset()
{
    time_reset = std::chrono::high_resolution_clock::now();
}


void TimeMeterNS::Reset()
{
    time_reset = std::chrono::high_resolution_clock::now();
}


float TimeMeterNS::ElapsedNS() const
{
    auto time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(time - time_reset);
    return (float)duration.count();
}


float TimeMeterMS::ElapsedMS() const
{
    auto time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time - time_reset);
    return (float)duration.count();
}


int64 Timer::CurrentTimeMS()
{
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

    return duration.count();
}


uint Timer::CurrentTimeSec()
{
    return (uint)(CurrentTimeMS() / 1000);
}


void Timer::PauseOnMS(uint timeMS)
{
    int64 time_end = CurrentTimeMS() + timeMS;

    while (CurrentTimeMS() < time_end)
    {
    }
}
