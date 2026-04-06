// 2025/10/06 11:10:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/RingBuffer.h"
#include <cstring>
#include <cctype>
#include <stm32f4xx_hal.h>


namespace HAL_USART1
{
    static UART_HandleTypeDef h =
    {
        USART1,
        {
            921600,
            UART_WORDLENGTH_8B,
            UART_STOPBITS_1,
            UART_PARITY_NONE,
            UART_MODE_TX_RX,
            UART_HWCONTROL_NONE,
            UART_OVERSAMPLING_16
        }
    };

    void *handle = &h;

    static uint8 buffer = 0;

    static RingBuffer in_buffer;
}


void HAL_USART1::Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    GPIO_InitStruct.Pin = GPIO_PIN_10;                  // RX
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;                   // TX
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    HAL_UART_Init(&h);

    HAL_UART_Receive_IT(&h, &buffer, 1);
}


void HAL_USART1::Transmit(const void *_buffer, int size)
{
    HAL_UART_Transmit(&h, (const uint8 *)_buffer, (uint16)size, 100);
}


void HAL_USART1::TransmitString(pchar str)
{
    Transmit(str, (int)std::strlen(str) + 1);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *_handle)
{
    HAL_USART1::in_buffer.Append(HAL_USART1::buffer);

    HAL_UART_Receive_IT(_handle, &HAL_USART1::buffer, 1);
}


void HAL_USART1::GetData(BufferOSDP &out_buffer)
{
    while (!in_buffer.IsEmpty())
    {
        char symbol = (char)in_buffer.Pop();

        symbol = (char)std::toupper(symbol);

        out_buffer.Append(symbol);
    }
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *_handle)
{
    // 1. —охран€ем информацию об ошибке (дл€ диагностики)
    uint32_t errors = _handle->ErrorCode;
    
    // 2. ќстанавливаем UART (это ќЅя«ј“≈Ћ№Ќџ… шаг!)
    HAL_UART_Abort(_handle);
    // »Ћ», если используете прием:
    // HAL_UART_AbortReceive(huart);
    // HAL_UART_AbortTransmit(huart);
    
    // 3. ќчищаем флаги ошибок в регистрах UART
    __HAL_UART_CLEAR_FLAG(_handle, UART_FLAG_ORE | UART_FLAG_NE | UART_FLAG_PE | UART_FLAG_FE);
    
    // 4. ƒекодируем ошибку дл€ диагностики (опционально, но очень полезно)
    if(errors & HAL_UART_ERROR_ORE) {
        // Overrun Error - данные тер€ютс€, слишком высока€ скорость обработки
        // Ќужно оптимизировать код или использовать DMA
    }
    if(errors & HAL_UART_ERROR_NE) {
        // Noise Error - помехи на линии
    }
    if(errors & HAL_UART_ERROR_FE) {
        // Framing Error - неверный стоп-бит, проблема синхронизации
    }
    if(errors & HAL_UART_ERROR_PE) {
        // Parity Error - ошибка четности
    }
    
    // 5. ѕереинициализируем UART
    HAL_UART_DeInit(_handle);
    HAL_UART_Init(_handle);
    
    // 6. ѕ≈–≈«јѕ”— ј≈ћ прием данных!
    // Ёто  –»“»„≈— » важный шаг!
    HAL_UART_Receive_IT(_handle, &HAL_USART1::buffer, 1);
}
