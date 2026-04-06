// 2025/09/22 13:39:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL_PINS.h"


// ПЛИС


namespace FPGA
{
    void Init();

    void PeriodicTask();

    void StartScan(uint periodMS);

    void StopScan();

    // Развёртка запущена
    bool IsScanning();

    namespace Reg
    {
        void SetLength(int num, uint length);
        void Write(int num, uint value);
    }
}
