// 2026/07/23 16:19:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Структуры для аппаратной информации GPIO (нужны для Linux)
struct gpiod_chip;
struct gpiod_line;


struct HardwarePinInfo
{
    int pin_number;          // Номер GPIO пина чипа
    const char *chip_name;   // Имя GPIO чипа
    gpiod_chip *chip;        // Дескриптор чипа
    gpiod_line *line;        // Дескриптор линии
    pchar name_connector;    // Так он обозначен на разъёме
};


struct OutputPinInfo
{
    HardwarePinInfo hw;
};


struct InputPinInfo
{
    HardwarePinInfo hw;
    bool last_state;                    // Последнее состояние
    std::function<void(bool)> callback; // Callback функция
};


namespace PinInfo
{
    InputPinInfo GetInputPinInfo(int num_pin);
}
