// 2025/11/27 15:47:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Device.h"
#include "Device/Commutator.h"
#include "Device/FPGA.h"
#include "Device/Sources.h"
#include "Device/Channels.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"


void Device::Init()
{
    pDAT1_DAC.Init();
    pDAT1_DAC.ToLow();

    pCLK1_DAC.Init();
    pCLK1_DAC.ToLow();

    pDAT2_DAC.Init();
    pDAT2_DAC.ToLow();

    pCLK2_DAC.Init();
    pCLK2_DAC.ToLow();

    Timer::Init();

    FPGA::Init();

    Commutator::Init();

    Source50V::Init();

    Source3kV::Init();

    ChanB::Init();

    ChanS::Init();

    ChanC::Init();
}


void Device::PeriodicTask()
{
    HAL_TIM2::StartMultiMeasurement();

    Commutator::PeriodicTask();

    FPGA::PeriodicTask();
}


void Device::EmergencyStop()
{
    ChanC::EmergencyStop();
}


void Device::EmergencyStart()
{
    ChanC::EmergencyStart();
}
