// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Debug.h"
#include "Hardware/Timer.h"



//static uint timeStart;
//static uint timePrev;
static uint timeCounter;
//static uint timeStartCounter;

// Время старта профилировщика
//volatile static uint timeStartMS = 0;
// Время предыдущей точки профилировщика
//volatile static uint timePrevMS = 0;

const char *Debug::file[10] = { nullptr };
int   Debug::line[10] = { 0 };


void Debug::StartProfilingMS()
{
//    timeStartMS = TIME_MS;
//    timePrevMS = TIME_MS; //-V656
}


uint Debug::PointProfilingMS(const char * /*_file*/, int /*_line*/)
{
//   uint d = TIME_MS - timePrevMS;
//   timePrevMS = TIME_MS;
//
//   return d;
    
    return 0;
}


void Debug::StartProfilingUS()
{
//    timeStart = TIME_US;
//    timePrev = timeStart;
}


uint Debug::PointProfilingUS(const char * /*_file*/, int /*_line*/)
{
//    uint d = TIME_US - timePrev;
//    timePrev = TIME_US;
//    return d;
    
    return 0;
}


void Debug::ClearTimeCounter()
{
    timeCounter = 0;
}


void Debug::StartIncreaseCounter()
{
//    timeStartCounter = TIME_US;
}


void Debug::StopIncreaseCounter()
{
//    timeCounter += (TIME_US - timeStartCounter);
}


uint Debug::GetTimeCounterUS()
{
    return timeCounter;
}
