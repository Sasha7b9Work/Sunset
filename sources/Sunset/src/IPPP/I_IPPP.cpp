// 2026/03/10 16:47:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/I_IPPP.h"
#include "IPPP/IDevice.h"
#include "IPPP/Real/RealDevice.h"
#include "IPPP/Keyboard/Keyboard.h"
#include "Communicator/GPIO/GPIO.h"
#include "IPPP/Real/RealIPPP.h"
#include "IPPP/Emulator/EmulatorDevice.h"
#include "IPPP/Emulator/EmulatorIPPP.h"
#include "Settings/Settings.h"


std::unique_ptr<I_IPPP> I_IPPP::instance;


void I_IPPP::SetInstance(std::unique_ptr<I_IPPP> device)
{
    instance = std::move(device);

    if (SET::emulate_mode->Get())
    {
        IDevice::impl = new EmulatorDevice();
    }
    else
    {
        IDevice::impl = new RealDevice();

        Keyboard::Init();
    }

    IDevice::impl->Init();
}


I_IPPP *I_IPPP::GetInstance()
{
    return instance.get();
}
