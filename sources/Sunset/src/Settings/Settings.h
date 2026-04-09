// 2023/08/09 19:11:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Value.h"


// Настройки прибора
namespace SET
{
    void AppendValue(ValuePoint *);
    void AppendValue(ValueBool *);
    void AppendValue(ValueInt *);
    void AppendValue(ValueUInt *);

    void Init();

    void Load();
    void Save();

    extern ValueBool        *debug_mode;
    extern ValueBool        *emulate_mode;              // Режим эмуляции

    // Настройки ГУИ
    namespace GUI
    {
        extern ValuePoint       *pos_console;
        extern ValuePoint       *size_console;
        extern ValueBool        *maximized_console;
        extern ValueInt         *serial_port_num;       // Нумерация начинается с нуля. 0 == COM1

        extern ValueUInt        *color_background;
        extern ValueUInt        *color_grid;
        extern ValueUInt        *color_font;
        extern ValueUInt        *color_curve;           // Цвет графиков
        extern ValueUInt        *color_link;
        extern ValueUInt        *color_secant;          // Цвет секущей

        extern ValueInt         *size_point;            // Размер точки на графике
        extern ValueInt         *current_panel;
    }
};
