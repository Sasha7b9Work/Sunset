// 2025/09/10 21:36:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Emulator/EmulatorIPPP.h"
#include "Application.h"


void EmulatorIPPP::PeriodicTask()
{
    if (in_process_measuring)
    {
        int data[5][MAX_NUMBER_POINTS];

        if (ReadData(data))
        {
        }
    }
}


void EmulatorIPPP::PressButtonStart()
{
    in_process_measuring = true;
}


void EmulatorIPPP::PressButtonStop()
{
    in_process_measuring = false;
}


bool EmulatorIPPP::ReadData(int [5][MAX_NUMBER_POINTS])
{
    return false;
}
