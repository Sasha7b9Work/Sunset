// 2023/08/09 19:11:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Utils/Configurator.h"
#include "Application.h"


namespace SET
{
    ValueBool            *debug_mode = nullptr;
    ValueBool            *emulate_mode = nullptr;

    namespace GUI
    {
        ValuePoint       *pos_console = nullptr;
        ValuePoint       *size_console = nullptr;
        ValueBool        *maximized_console = nullptr;
        ValueInt         *serial_port_num = nullptr;

        ValueUInt        *color_background = nullptr;
        ValueUInt        *color_grid = nullptr;
        ValueUInt        *color_font = nullptr;
        ValueUInt        *color_curve = nullptr;
        ValueUInt        *color_link = nullptr;
        ValueUInt        *color_secant = nullptr;

        ValueInt         *size_point = nullptr;
        Value<wxString>  *current_panel = nullptr;

        ValueInt         *mode_application = nullptr;

        void Load();
        void Save();
    }


    // \warn Такое хитрое обращение к векторам сделано потому, что порядок инициализации глобальных статических объектов неопределён,
    // из-за чего к моменту, когда в эти вектора кладутся значения, оин могут быть не готовы к работе. 
    static std::vector<ValuePoint *> &VectorPoint()
    {
        static std::vector<ValuePoint *> vec;
        return vec;
    }

    static std::vector<ValueBool *> &VectorBool()
    {
        static std::vector<ValueBool *> vec;
        return vec;
    }

    static std::vector<ValueInt *> &VectorInt()
    {
        static std::vector<ValueInt *> vec;
        return vec;
    }

    static std::vector<ValueUInt *> &VectorUInt()
    {
        static std::vector<ValueUInt *> vec;
        return vec;
    }
}


void SET::Init()
{
    debug_mode = new ValueBool("debug_mode", false);
    emulate_mode = new ValueBool("emulate_mode", false);

    GUI::pos_console = new ValuePoint("gui_pos_console", { 10, 10 });
    GUI::size_console = new ValuePoint("gui_size_console", { 400, 400 });
    GUI::maximized_console = new ValueBool("gui_maximized_console", false);
    GUI::serial_port_num = new ValueInt("serial_port_num", 0);

    GUI::color_background = new ValueUInt("color_background", 0xFFFFFF);
    GUI::color_grid = new ValueUInt("color_grid", 0);
    GUI::color_font = new ValueUInt("color_font", 0);
    GUI::color_curve = new ValueUInt("color_curve", 0);
    GUI::color_link = new ValueUInt("color_link", 0);
    GUI::color_secant = new ValueUInt("color_secant", 0);

    GUI::size_point = new ValueInt("size_point", 2);
    GUI::current_panel = new Value<wxString>{ "current_panel", "PanelChannelC" };

    GUI::mode_application = new ValueInt("mode_application", (int)Application::Mode::Old);
}


void SET::AppendValue(ValuePoint *value)
{
    VectorPoint().push_back(value);
}


void SET::AppendValue(ValueBool *value)
{
    VectorBool().push_back(value);
}


void SET::AppendValue(ValueInt *value)
{
    VectorInt().push_back(value);
}


void SET::AppendValue(ValueUInt *value)
{
    VectorUInt().push_back(value);
}


void SET::Load()
{
    for (auto elem : VectorPoint())
    {
        elem->Load();
    }

    for (auto elem : VectorBool())
    {
        elem->Load();
    }

    for (auto elem : VectorInt())
    {
        elem->Load();
    }

    for (auto elem : VectorUInt())
    {
        elem->Load();
    }

    GUI::Load();
}


void SET::Save()
{
    for (auto elem : VectorPoint())
    {
        elem->Save();
    }

    for (auto elem : VectorBool())
    {
        elem->Save();
    }

    for (auto elem : VectorInt())
    {
        elem->Save();
    }

    for (auto elem : VectorUInt())
    {
        elem->Save();
    }

    GUI::Save();
}


void SET::GUI::Load()
{
    current_panel->Load();
}


void SET::GUI::Save()
{
    current_panel->Save();
}
