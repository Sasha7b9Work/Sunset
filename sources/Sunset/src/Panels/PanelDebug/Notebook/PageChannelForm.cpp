// 2025/10/07 11:51:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageChannelForm.h"
#include "Panels/PanelDebug/Notebook/Register.h"


/*
    ADC     AD7691      16 разрядов
    DAC     AD5543      12 разрядов
    Регистр. упр.       32 разрядов
*/


PageChannelForm *PageChannelForm::self = nullptr;


PageChannelForm::PageChannelForm(wxNotebook *parent) :
    PageChip(parent, L("Формирователь"))
{
    self = this;

    {
        RegAD5443 *dac1 = new RegAD5443(this, dacs[DAC::_1_ChannelC_Meas], L("Измеритель"));

        AppendRegister(dac1);
    }

    {
        RegFPGA *fpga2 = new RegFPGA(this, regs[REG::_2_ChannelC]);

        std::vector<StructDescription> desc0;
        desc0.reserve(6);

        std::vector<StructDescription::CommandStruct> commandQ2;
        commandQ2.reserve(2);
        commandQ2.emplace_back(StructDescription::CommandStruct{ 0b0, L("x 20 вкл") });
        commandQ2.emplace_back(StructDescription::CommandStruct{ 0b1, L("x 20 выкл") });
        desc0.emplace_back(StructDescription{ 2, 1, "Q2", "x20", {false, commandQ2} });

        std::vector<StructDescription::CommandStruct> commandQ3;
        commandQ3.reserve(2);
        commandQ3.emplace_back(StructDescription::CommandStruct{ 0b0, L("x 5 вкл") });
        commandQ3.emplace_back(StructDescription::CommandStruct{ 0b1, L("x 5 выкл") });
        desc0.emplace_back(StructDescription{ 3, 1, "Q3", "x5", {false, commandQ3} });

        std::vector<StructDescription::CommandStruct> commandQ4;
        commandQ4.reserve(2);
        commandQ4.emplace_back(StructDescription::CommandStruct{ 0b0, L("x 1 вкл") });
        commandQ4.emplace_back(StructDescription::CommandStruct{ 0b1, L("x 1 выкл") });
        desc0.emplace_back(StructDescription{ 4, 1, "Q4", "x1", {false, commandQ4} });

        std::vector<StructDescription::CommandStruct> commandQ5;
        commandQ5.reserve(2);
        commandQ5.emplace_back(StructDescription::CommandStruct{ 0b0, L(": 5 вкл") });
        commandQ5.emplace_back(StructDescription::CommandStruct{ 0b1, L(": 5 выкл") });
        desc0.emplace_back(StructDescription{ 5, 1, "Q5", ":5", {false, commandQ5} });

        std::vector<StructDescription::CommandStruct> commandQ6;
        commandQ6.reserve(2);
        commandQ6.emplace_back(StructDescription::CommandStruct{ 0b0, L(": 1.25 вкл") });
        commandQ6.emplace_back(StructDescription::CommandStruct{ 0b1, L(": 1.25 выкл") });
        desc0.emplace_back(StructDescription{ 6, 1, "Q6", ":1.25", {false, commandQ6} });

        std::vector<StructDescription::CommandStruct> commandQ7;
        commandQ7.reserve(2);
        commandQ7.emplace_back(StructDescription::CommandStruct{ 0b0, L(": 250 вкл") });
        commandQ7.emplace_back(StructDescription::CommandStruct{ 0b1, L(": 250 выкл") });
        desc0.emplace_back(StructDescription{ 7, 1, "Q7", ":250", {false, commandQ7} });

        fpga2->SetDescriptionBits(0, desc0);

        AppendRegister(fpga2);
    }

    {
        RegAD5443 *dac0 = new RegAD5443(this, dacs[DAC::_0_ChannelC_Form], L("Формирователь. Управление с Orange Pi"));

        AppendRegister(dac0);
    }
}
