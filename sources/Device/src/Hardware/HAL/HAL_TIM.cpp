// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


uint HAL_TIM::TimeMS()
{
    return HAL_GetTick();
}


void HAL_TIM::Delay(uint timeMS)
{
    HAL_Delay(timeMS);
}
