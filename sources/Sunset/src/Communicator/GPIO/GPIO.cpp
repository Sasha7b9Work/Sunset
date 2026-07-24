// 2025/10/09 08:37:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/GPIO/GPIO.h"
#include "Utils/Timer.h"
#include <thread>


// Lin specific
#include <gpiod.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>


PinIn pinFIFO_FULL(Pin::FULL_36);
PinIn pinSTART(Pin::KN_START_15);
PinIn pinSTOP(Pin::KN_STOP_33);
PinIn pinKA(Pin::KN_A_11);
PinIn pinKB(Pin::KN_B_13);


namespace GPIO
{
    static PinInfo pins[] =
    {
        PinStorage::GetInputPinInfo(Pin::T13_03),
        PinStorage::GetInputPinInfo(Pin::T14_05),
        PinStorage::GetInputPinInfo(Pin::T15_07),
        PinStorage::GetInputPinInfo(Pin::KN_A_11),
        PinStorage::GetInputPinInfo(Pin::T17_12),
        PinStorage::GetInputPinInfo(Pin::KN_B_13),
        PinStorage::GetInputPinInfo(Pin::KN_START_15),
        PinStorage::GetInputPinInfo(Pin::DT_DDAC_16_out),
        PinStorage::GetInputPinInfo(Pin::CLK_DDAC_18_out),
        PinStorage::GetInputPinInfo(Pin::F_CON2_22),
        PinStorage::GetInputPinInfo(Pin::F_CON1_26),
        PinStorage::GetInputPinInfo(Pin::ENB_PC_27),
        PinStorage::GetInputPinInfo(Pin::EN_DDA1_31_out),
        PinStorage::GetInputPinInfo(Pin::ST_EXT_32),
        PinStorage::GetInputPinInfo(Pin::KN_STOP_33),
        PinStorage::GetInputPinInfo(Pin::EN_DDA2_35_out),
        PinStorage::GetInputPinInfo(Pin::FULL_36),
        PinStorage::GetInputPinInfo(Pin::FIT_37)
    };

    constexpr size_t PINS_COUNT = std::size(pins);

    // Вспомогательные функции для получения информации о пинах
    PinInfo *GetPinInfo(Pin::E type)
    {
        for (size_t i = 0; i < PINS_COUNT; i++)
        {
            if (pins[i].pin == type)
            {
                return &pins[i];
            }
        }

        return nullptr;
    }
}


namespace GPIO
{
    void Init()
    {
        LOG_WRITE("Initializing GPIO...");

        for (size_t i = 0; i < PINS_COUNT; i++)
        {
            PinInfo &info = pins[i];

            if (info.is_input)
            {
                info.hw.chip = gpiod_chip_open_by_name(info.hw.chip_name);
                if (!info.hw.chip)
                {
                    LOG_ERROR("Cannot open GPIO chip %s", info.hw.chip_name);
                    continue;
                }

                info.hw.line = gpiod_chip_get_line(info.hw.chip, (uint)info.hw.pin_number);
                if (!info.hw.line)
                {
                    LOG_ERROR("Cannot get GPIO line %d", info.hw.pin_number);
                    gpiod_chip_close(info.hw.chip);
                    info.hw.chip = nullptr;
                    continue;
                }

                int ret = gpiod_line_request_input_flags(info.hw.line, nullptr,
                    GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_DOWN);
                if (ret < 0)
                {
                    LOG_ERROR("Cannot request GPIO line %d as input", info.hw.pin_number);
                    gpiod_chip_close(info.hw.chip);
                    info.hw.chip = nullptr;
                    info.hw.line = nullptr;
                    continue;
                }

                info.last_state = (gpiod_line_get_value(info.hw.line) == 1);

                LOG_WRITE("GPIO input pin %s:%d initialized", info.hw.chip_name, info.hw.pin_number);
            }
            else
            {
                info.hw.chip = gpiod_chip_open_by_name(info.hw.chip_name);
                if (!info.hw.chip)
                {
                    LOG_ERROR("Cannot open GPIO chip %s", info.hw.chip_name);
                    continue;
                }

                info.hw.line = gpiod_chip_get_line(info.hw.chip, (uint)info.hw.pin_number);
                if (!info.hw.line)
                {
                    LOG_ERROR("Cannot get GPIO line for pin %d", info.hw.pin_number);
                    gpiod_chip_close(info.hw.chip);
                    info.hw.chip = nullptr;
                    continue;
                }

                int ret = gpiod_line_request_output(info.hw.line, nullptr, 0);
                if (ret < 0)
                {
                    LOG_ERROR("Cannot request GPIO line for pin %d as output", info.hw.pin_number);
                    gpiod_chip_close(info.hw.chip);
                    info.hw.chip = nullptr;
                    info.hw.line = nullptr;
                    continue;
                }

                LOG_WRITE("GPIO output pin %s:%d initialized", info.hw.chip_name, info.hw.pin_number);

            }
        }
    }

    void DeInit()
    {
        LOG_WRITE("Deinitializing GPIO...");

        for (size_t i = 0; i < PINS_COUNT; i++)
        {
            PinInfo &info = pins[i];

            if (info.is_input)
            {
                if (info.hw.line)
                {
                    gpiod_line_release(info.hw.line);
                    info.hw.line = nullptr;
                }

                if (info.hw.chip)
                {
                    gpiod_chip_close(info.hw.chip);
                    info.hw.chip = nullptr;
                }
            }
            else
            {
                if (info.hw.line)
                {
                    gpiod_line_release(info.hw.line);
                    info.hw.line = nullptr;
                }

                if (info.hw.chip)
                {
                    gpiod_chip_close(info.hw.chip);
                    info.hw.chip = nullptr;
                }
            }
        }

        LOG_WRITE("GPIO deinitialized");
    }
}


bool PinIn::GetHardware(gpiod_line *line)
{
    return gpiod_line_get_value(line) == 1;
}


bool Pin::Get() const
{

    PinInfo *info = GPIO::GetPinInfo(type_);

    if (info && info->hw.line)
    {
        int val = gpiod_line_get_value(info->hw.line);

        if (val < 0)
        {
            LOG_ERROR("Error: Cannot read GPIO pin number %d", info->hw.pin_number);
            return false;
        }

        return (val == 1);
    }

    return false;
}


void PinOut::Set(bool state)
{
    PinInfo *info = GPIO::GetPinInfo(type_);

    if (info && info->hw.line)
    {
        if (info->is_input)
        {
            LOG_ERROR("Pin is input");
        }
        else
        {
            int ret = gpiod_line_set_value(info->hw.line, state ? 1 : 0);

            if (ret < 0)
            {
                LOG_ERROR("Error: Cannot set GPIO pin %d to %s", info->hw.pin_number, state ? "HIGH" : "LOW");
            }
        }
    }
}


void PinOut::Set(gpiod_line *line, int state)
{
    gpiod_line_set_value(line, state);
}


void PinOut::ToHi()
{
    Set(true);
}


void PinOut::ToLow()
{
    Set(false);
}
