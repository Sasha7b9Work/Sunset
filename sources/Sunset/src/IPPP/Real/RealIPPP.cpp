// 2025/09/10 21:28:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Real/RealIPPP.h"
#include "IPPP/IDevice.h"
#include "IPPP/Emulator/EmulatorDevice.h"
#include "Communicator/GPIO/GPIO.h"
#include "IPPP/Keyboard/Keyboard.h"
#include "IPPP/Real/RealDevice.h"
#include "Utils/Timer.h"


bool RealIPPP::IsChanBS(const Chan &ch) const
{
    if (ch.IsBS())
    {
        return true;
    }

    LOG_ERROR("Channel must be B or S, not %s", ch.Name());

    return false;
}



void RealIPPP::PeriodicTask()
{
    Keyboard::PeriodicTask();

    IDevice::impl->PeriodicTask();

    int data[5][MAX_NUMBER_POINTS];

    if (ReadData(data))
    {
        for (int i = 0; i < 5; i++)
        {
        }
    }
}


void RealIPPP::PressButtonStart()
{

}


void RealIPPP::PressButtonStop()
{

}


bool RealIPPP::ReadData(int /*data*/[5][MAX_NUMBER_POINTS])
{
    bool result = false;

    /*
    static bool prev = false;

    if (pinFIFO_FULL.Get() && prev == false)
    {
        gpiod_line *infoMOSI = GPIO::GetInputPinInfo(Pin::In_DAT_F0)->hw.line;
        gpiod_line *infoCS = GPIO::GetOutputPinInfo(Pin::Out_SPI_CS)->hw.line;
        gpiod_line *infoREQ = GPIO::GetOutputPinInfo(Pin::Out_REQ_RD)->hw.line;

        for (int i = 0; i < MAX_NUMBER_POINTS; i++)
        {
            PinOut::Set(infoCS, 0);

            for (int num_adc = 0; num_adc < 4; num_adc++)
            {
                int value = 0;

                for (int num_bit = 15; num_bit >= 0; num_bit--)
                {
                    PinOut::Set(infoREQ, 1);

                    if (PinIn::GetHardware(infoMOSI))
                    {
                        value |= (1 << num_bit);
                    }

                    PinOut::Set(infoREQ, 0);
                }

                data[num_adc][i] = value;
            }

            int value = 0;

            for (int num_bit = 7; num_bit >= 0; num_bit--)
            {
                PinOut::Set(infoREQ, 1);

                if (PinIn::GetHardware(infoMOSI))
                {
                    value |= (1 << num_bit);
                }

                PinOut::Set(infoREQ, 0);
            }

            data[4][i] = value;

            PinOut::Set(infoCS, 1);
        }

        result = true;
    }

    prev = pinFIFO_FULL.Get();
    */

    return result;
}


void RealIPPP::Pause()
{
    volatile int i = 0;

    for (i = 0; i < 1000; i += 1)
    {
    }
}
