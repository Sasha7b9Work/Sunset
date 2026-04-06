// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


namespace HAL_TIM2
{
    static TIM_HandleTypeDef handler;
}


void HAL_TIM2::Init()
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    handler.Instance = TIM2;
    handler.Init.Prescaler = 0;
    handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    handler.Init.Period = (uint)(-1);
    handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&handler);

    HAL_TIM_Base_Start(&handler);
}


uint HAL_TIM2::GetTicks()
{
    return TIM2->CNT;
}


void HAL_TIM2::StartMultiMeasurement()
{
    TIM2->CR1 &= (uint)~TIM_CR1_CEN;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
}


uint HAL_TIM2::TimeUS()
{
    return (TIM2->CNT / 60);
}
