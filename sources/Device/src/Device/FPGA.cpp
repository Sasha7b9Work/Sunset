// 2025/09/22 13:39:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/FPGA.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"


namespace FPGA
{
    static PinIn  pinLIMIT(Port::_E, Pin::_7);          // pin58
    static PinOut pinA0_RG(Port::_G, Pin::_5);          // pin90
    static PinOut pinA1_RG(Port::_G, Pin::_6);          // pin91
    static PinOut pinCLK_RG(Port::_G, Pin::_7);         // pin92
    static PinOut pinWR_RG(Port::_G, Pin::_8);          // pin93
    static PinOut pinDAT_RG(Port::_D, Pin::_13);        // pin82
    static PinOut pinSTART_TB(Port::_A, Pin::_2);       // pin36
    static PinOut pinSTOP_TB(Port::_F, Pin::_5);        // pin15

    static uint lengths[10] = { 9, 8, 8, 0, 0, 0, 0, 0, 0, 0 };

    // Дать start FPGA
    static void WriteStart();

    // Дать стоп FPGA
    static void WriteStop();

    static TimeMeterMS meter_scan;          // По этому счётчику отсчитываем развёртку

    static bool is_running_scan = false;    // Если true - идёт развёртка

    static uint period_scan = 1000;         // Период запуска развёртки

    static void Pause();

    namespace Reg
    {
        static const int NUM_REGS = 3;

        static uint values[NUM_REGS] = { 0, 0, 0 };

        static void WriteRAW(int num, uint value);

        static void WriteAll();
    }
}


void FPGA::Init()
{
    pinLIMIT.Init();
    pinA0_RG.Init();
    pinA1_RG.Init();

    pinCLK_RG.Init();
    pinCLK_RG.ToLow();

    pinWR_RG.Init();
    pinWR_RG.ToLow();

    pinDAT_RG.Init();

    pinSTART_TB.Init();
    pinSTART_TB.ToLow();

    pinSTOP_TB.Init();
    pinSTOP_TB.ToLow();

    WriteStop();
}


void FPGA::PeriodicTask()
{
    if (!is_running_scan)
    {
        return;
    }

    if (meter_scan.ElapsedMS() >= period_scan)
    {
        meter_scan.Reset();

        WriteStart();
    }
}


void FPGA::StartScan(uint periodMS)
{
    period_scan = periodMS;

    is_running_scan = true;

    Reg::WriteAll();

    WriteStart();
}


void FPGA::StopScan()
{
    is_running_scan = false;

    WriteStop();
}


bool FPGA::IsScanning()
{
    return is_running_scan;
}


void FPGA::WriteStart()
{
    Pause();
    pinSTART_TB.ToHi();
    Pause();
    pinSTART_TB.ToLow();
    meter_scan.Reset();
    Pause();
}


void FPGA::WriteStop()
{
    Pause();
    pinSTOP_TB.ToHi();
    Pause();
    pinSTOP_TB.ToLow();
    Pause();
}


void FPGA::Reg::SetLength(int num, uint length)
{
    lengths[num] = length;
}


void FPGA::Reg::WriteRAW(int num, uint value)
{
    if (num < 0 || num >= NUM_REGS)
    {
        return;
    }

    Pause();

    pinA0_RG.Set(_GET_BIT(num, 0) != 0);
    pinA1_RG.Set(_GET_BIT(num, 1) != 0);

    int length = (int)lengths[num];

    for (int bit = length - 1; bit >= 0; bit--)
    {
        pinDAT_RG.Set(_GET_BIT(value, bit) != 0);
        pinCLK_RG.ToHi();
        Pause();
        pinCLK_RG.ToLow();
    }

    pinWR_RG.ToHi();
    Pause();
    pinWR_RG.ToLow();

    Pause();
}


void FPGA::Reg::Write(int num, uint value)
{
    if (num >= 0 && num < NUM_REGS)
    {
        values[num] = value;

        if (is_running_scan)
        {
            WriteRAW(num, values[num]);
        }
    }
}


void FPGA::Reg::WriteAll()
{
    for (int i = 0; i < 1; i++)
    {
        WriteRAW(i, values[i]);
    }
}


void FPGA::Pause()
{
    volatile int i = 20;

    while (i > 0)
    {
        i--;
    }
}
