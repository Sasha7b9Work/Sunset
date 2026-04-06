#pragma once
#include "IPPP/Tests/SettingsTests.h"
#include "IPPP/Tests/Ranges.h"


// Здесь идёт обмен с контроллером

class IDevice
{
public:

    static IDevice *impl;

    virtual ~IDevice() = default;

    virtual bool Init() = 0;

    virtual void PeriodicTask() = 0;

    virtual void Shutdown() = 0;

    virtual bool IsConnected() const = 0;

    // Заслать команду и завершить нулём
    virtual void SendCommand(pchar format, ...) const = 0;
};


/*
    :FPGA[0...2]:LENGTH [1...32]    Установка длины регистра FPGA
    :FPGA[0...2]:WRITE XXXXXh       Запись шестнадцатиричного значения в регистр FPGA
    :SCAN:START  Xms                Запуск развёртки с периодом X ms
    :SCAN:STOP                      Останов развёртки
*/
