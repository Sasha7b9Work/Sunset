// 2025/08/30 18:39:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL_PINS.h"
#include <stm32f4xx_hal.h>


PinOut pDAT1_DAC(Port::_D, Pin::_8);
PinOut pCLK1_DAC(Port::_D, Pin::_11);

PinOut pDAT2_DAC(Port::_B, Pin::_15);
PinOut pCLK2_DAC(Port::_D, Pin::_9);


Pin::Pin(Port::E _port, Pin::E _pin, PinMode::E _mode)
{
    static GPIO_TypeDef *ports[Port::Count] =
    {
        GPIOA,
        GPIOB,
        GPIOC,
        GPIOD,
        GPIOE,
        GPIOF,
        GPIOG,
        GPIOH,
        GPIOI
    };

    port = ports[_port];

    static const uint16 pins[Pin::Count] =
    {
        GPIO_PIN_0,
        GPIO_PIN_1,
        GPIO_PIN_2,
        GPIO_PIN_3,
        GPIO_PIN_4,
        GPIO_PIN_5,
        GPIO_PIN_6,
        GPIO_PIN_7,
        GPIO_PIN_8,
        GPIO_PIN_9,
        GPIO_PIN_10,
        GPIO_PIN_11,
        GPIO_PIN_12,
        GPIO_PIN_13,
        GPIO_PIN_14,
        GPIO_PIN_15
    };

    pin = pins[_pin];

    mode = _mode;
}


void PinIn::Init()
{
    GPIO_InitTypeDef is =
    {
        pin,
        GPIO_MODE_INPUT,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_HIGH,
        0
    };

    HAL_GPIO_Init((GPIO_TypeDef *)port, &is);

    prev_state = Get();
}


void PinOut::Init()
{
    GPIO_InitTypeDef is =
    {
        pin,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_HIGH,
        0
    };

    HAL_GPIO_Init((GPIO_TypeDef *)port, &is);
}


bool PinIn::Get()
{
    GPIO_PinState state = HAL_GPIO_ReadPin((GPIO_TypeDef *)port, pin);

    prev_state = (state != GPIO_PIN_RESET);

    return prev_state;
}


bool PinIn::PrevState() const
{
    return prev_state;
}


void PinOut::Set(bool value)
{
    HAL_GPIO_WritePin((GPIO_TypeDef *)port, pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void PinOut::ToLow()
{
    Set(false);
}


void PinOut::ToHi()
{
    Set(true);
}
