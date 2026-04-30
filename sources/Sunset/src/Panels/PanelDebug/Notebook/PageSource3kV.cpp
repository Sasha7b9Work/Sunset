// 2025/10/07 11:39:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageSource3kV.h"
#include "Panels/PanelDebug/Notebook/Register.h"

/*
    Регистр управления 24 разряда
*/

PageSource3kV *PageSource3kV::self = nullptr;


PageSource3kV::PageSource3kV(wxNotebook *parent) :
    PageChip(parent, "Источник 3 кВ")
{
    self = this;

    Register *reg0 = new RegFPGA(this, regs[0]);

    std::vector<StructDescription> desc0;

    std::vector<StructDescription::CommandStruct> commandsPolarity;
    commandsPolarity.emplace_back(StructDescription::CommandStruct{ 0b0, "Положительная" });
    commandsPolarity.emplace_back(StructDescription::CommandStruct{ 0b1, "Отрицательная" });
    desc0.emplace_back(StructDescription{ 0, 1, "п", "полярность", { true, commandsPolarity } });

    std::vector<StructDescription::CommandStruct> commandsRanges;
    commandsRanges.emplace_back(StructDescription::CommandStruct{ 0b000, "Обрыв коллектора" });
    commandsRanges.emplace_back(StructDescription::CommandStruct{ 0b001, "Обрыв коллектора (Режима ОЖИДАНИЕ нет)" });
    commandsRanges.emplace_back(StructDescription::CommandStruct{ 0b010, "Диапазон 50 В" });
    commandsRanges.emplace_back(StructDescription::CommandStruct{ 0b011, "30 В" });
    commandsRanges.emplace_back(StructDescription::CommandStruct{ 0b100, "300 В" });
    commandsRanges.emplace_back(StructDescription::CommandStruct{ 0b101, "3000 В" });
    desc0.emplace_back(StructDescription{ 1, 3, "пределы", "пределы", { true, commandsRanges } });

    std::vector<StructDescription::CommandStruct> commandsMode;
    commandsMode.emplace_back(StructDescription::CommandStruct{ 0b0, "Выбор режима AC" });
    commandsMode.emplace_back(StructDescription::CommandStruct{ 0b1, "Выбор режима DC, IMP, SIN" });
    desc0.emplace_back(StructDescription{ 4, 1, "р", "режим", { true, commandsMode } });

    std::vector<StructDescription::CommandStruct> commandsFiltr;
    commandsFiltr.emplace_back(StructDescription::CommandStruct{ 0b0, "Отключён" });
    commandsFiltr.emplace_back(StructDescription::CommandStruct{ 0b1, "Включён" });
    desc0.emplace_back(StructDescription{ 5, 1, "ф", "фильтр", { true, commandsFiltr } });

    reg0->SetDescriptionBits(0, desc0);

    AppendRegister(reg0);
}
