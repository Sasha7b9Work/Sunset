// 2025/10/07 12:25:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageMeasCurrent.h"
#include "Panels/PanelDebug/Notebook/Register.h"

/*
    ADC     AD7691      16 разрядов
    Управление          16 разрядов
*/

PageMeasCurrent *PageMeasCurrent::self = nullptr;


PageMeasCurrent::PageMeasCurrent(wxNotebook *parent) :
    PageChip(parent, "Измеритель тока")
{
    self = this;

    RegFPGA *reg5 = new RegFPGA(this, regs[5]);

    std::vector<StructDescription> desc5;

    desc5.emplace_back(StructDescription{ 0, 3, "предел", "Установка предела 200нА/20мкА/2мА/200мА/20А/200А", { true }, });
    desc5.emplace_back(StructDescription{ 3, 2, "к1", "Установка коэффициента 1/2/4/8", { true },});
    desc5.emplace_back(StructDescription{ 5, 1, "к2", "Установка коэффициента 1/10" });
    desc5.emplace_back(StructDescription{ 6, 1, "ё", "Дополнительная ёмкость в режиме DC" });
    desc5.emplace_back(StructDescription{ 7, 1, "б", "Блокировка сигнала Limit I" });

    reg5->SetDescriptionBits(0, desc5);

    AppendRegister(reg5);
}

