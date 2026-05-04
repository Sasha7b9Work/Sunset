// 2025/08/29 18:05:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Emulator/EmulatorDevice.h"
#include "Communicator/GPIO/GPIO.h"
#include "Communicator/SPI/SPI.h"
#include "Communicator/UART/UART.h"
#include <cstdarg>


EmulatorDevice::~EmulatorDevice()
{
    Shutdown();
}


bool EmulatorDevice::Init()
{
    GPIO::Init();
//    SPI::Init();
    UART::Init(EmulatorDevice::CallbackOnReceive);

    LOG_ERROR("Not implemented");

    return false;
}


void EmulatorDevice::PeriodicTask()
{

}


void EmulatorDevice::CallbackOnReceive(uint8 *, int)
{

}


void EmulatorDevice::EmulatorDevice::Shutdown()
{
    LOG_ERROR("Not implemented");
}


bool EmulatorDevice::IsConnected() const
{
    LOG_ERROR("Not implemented");

    return false;
}


void EmulatorDevice::SendCommand(pchar format, ...) const
{
    char message[1024];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    std::strcat(message, "\0");
}
