#include "defines.h"
#include "IPPP/IDevice.h"
#include "Communicator/UART/UART.h"
#include "Communicator/GPIO/GPIO.h"
#include "Communicator/SPI/SPI.h"
#include "IPPP/Real/Chips.h"
#include "IPPP/SCPI/SCPI.h"
#include "Communicator/ComPort/ComPort.h"
#include "IPPP/Real/RealDevice.h"
#include <cstdarg>
#include <cstring>


IDevice *IDevice::impl = nullptr;


RealDevice::~RealDevice()
{
    Shutdown();
}

bool RealDevice::Init()
{
    GPIO::Init();
    SPI::Init();

    if(UART::Init(SCPI::OnEventCallback))
    {
        connected = true;
        running = true;
        return true;
    }

    return false;
}


void RealDevice::PeriodicTask()
{
    SCPI::PeriodicTask();
}


void RealDevice::Shutdown()
{
    running = false;

    UART::DeInit();

    connected = false;
}


bool RealDevice::IsConnected() const
{
    return connected;
}


void RealDevice::SendCommand(pchar format, ...) const
{
    char message[1024];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    std::strcat(message, "\0");

#ifdef WIN32
    ComPort::Send(message, (int)std::strlen(message) + 1);
#else
    UART::SendBuffer(message, (int)std::strlen(message) + 1);
#endif
}
