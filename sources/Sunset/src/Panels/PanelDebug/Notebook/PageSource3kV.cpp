// 2025/10/07 11:39:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageSource3kV.h"
#include "Panels/PanelDebug/Notebook/Register.h"

PageSource3kV *PageSource3kV::self = nullptr;


PageSource3kV::PageSource3kV(wxNotebook *parent) :
    PageChip(parent, "Источник 3 кВ")
{
    self = this;

    Register *reg0 = new RegFPGA(this, regs[REG::_0_Source3kV]);

    std::vector<StructDescription> desc0;

    std::vector<StructDescription::CommandStruct> commandsPolarity;
    commandsPolarity.emplace_back(StructDescription::CommandStruct{ 0b0, "Положительная" });
    commandsPolarity.emplace_back(StructDescription::CommandStruct{ 0b1, "Отрицательная" });
    desc0.emplace_back(StructDescription{ 0, 1, "п", "полярность выходного сигнала", { true, commandsPolarity } });

    std::vector<StructDescription::CommandStruct> commandsRanges;
    commandsRanges.emplace_back(StructDescription::CommandStruct{ 0b000, "Обрыв коллектора" });
    commandsRanges.emplace_back(StructDescription::CommandStruct{ 0b001, "+/- 30 В" });
    commandsRanges.emplace_back(StructDescription::CommandStruct{ 0b010, "+/- 300 В" });
    commandsRanges.emplace_back(StructDescription::CommandStruct{ 0b011, "+/- 3000 В" });
    desc0.emplace_back(StructDescription{ 1, 3, "пределы", "пределы", { true, commandsRanges } });

    std::vector<StructDescription::CommandStruct> commandsMode;
    commandsMode.emplace_back(StructDescription::CommandStruct{ 0b0, "AC" });
    commandsMode.emplace_back(StructDescription::CommandStruct{ 0b1, "DC, IMP, SIN" });
    desc0.emplace_back(StructDescription{ 4, 1, "р", "режим", { true, commandsMode } });

    std::vector<StructDescription::CommandStruct> commandsFiltr;
    commandsFiltr.emplace_back(StructDescription::CommandStruct{ 0b0, "Отключён" });
    commandsFiltr.emplace_back(StructDescription::CommandStruct{ 0b1, "Включён" });
    desc0.emplace_back(StructDescription{ 5, 1, "ф", "сглаживающий фильтр", { true, commandsFiltr } });

    std::vector<StructDescription::CommandStruct> commands300V;
    commands300V.emplace_back(StructDescription::CommandStruct{ 0b0, "Выключен" });
    commands300V.emplace_back(StructDescription::CommandStruct{ 0b1, "Включён" });
    desc0.emplace_back(StructDescription{ 6, 1, " ", "совместное включение диапазона источника 300 В и диапазона измерителя 200 мА", { true, commands300V } });

    std::vector<StructDescription::CommandStruct> commands30V;
    commands30V.emplace_back(StructDescription::CommandStruct{ 0b0, "Выключен" });
    commands30V.emplace_back(StructDescription::CommandStruct{ 0b1, "Включён" });
    desc0.emplace_back(StructDescription{ 7, 1, " ", "совместное включение диапазона источника 30 В и диапазона измерителя 200 мА", { true, commands30V} });

    reg0->SetDescriptionBits(0, desc0);

    AppendRegister(reg0);
}
