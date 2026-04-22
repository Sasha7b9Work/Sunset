 // 2025/10/07 11:46:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainPanel/PanelDebug/Notebook/PageCommutator.h"
#include "MainPanel/PanelDebug/Notebook/Register.h"


/*
    Регистр управления 16 разрядов
*/

PageCommutator *PageCommutator::self = nullptr;


PageCommutator::PageCommutator(wxNotebook *parent) :
    PageChip(parent, "Коммутатор")
{
    self = this;

    Register *reg1 = new RegFPGA(this, regs[1]);

    std::vector<StructDescription::CommandStruct> commandsBase; //-V827 //-V826
    commandsBase.emplace_back(StructDescription::CommandStruct{ 0b00000, "Обрыв" });
    commandsBase.emplace_back(StructDescription::CommandStruct{ 0b10001, "Коллектор" });
    commandsBase.emplace_back(StructDescription::CommandStruct{ 0b10010, "Эммитер" });
    commandsBase.emplace_back(StructDescription::CommandStruct{ 0b10100, "Эммитер 1кОм" });
    commandsBase.emplace_back(StructDescription::CommandStruct{ 0b11000, "Генератор ступенек" });

    std::vector<StructDescription::CommandStruct> commandsS; //-V827 //-V826
    commandsS.emplace_back(StructDescription::CommandStruct{ 0b00, "Обрыв" });
    commandsS.emplace_back(StructDescription::CommandStruct{ 0b01, "Эмиттера" });
    commandsS.emplace_back(StructDescription::CommandStruct{ 0b10, "Генератор ступенек" });

    std::vector<StructDescription> desc1;
    desc1.emplace_back(StructDescription{ 0, 5, "база", "база", { true, commandsBase } });
    desc1.emplace_back(StructDescription{ 5, 2, "подложка", "подложка", { true, commandsS }});
    reg1->SetDescriptionBits(0, desc1);

    AppendRegister(reg1);
}
