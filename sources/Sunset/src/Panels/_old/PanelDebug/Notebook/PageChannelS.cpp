// 2025/10/07 12:22:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageChannelS.h"


/*
    ADC     AD7691      16 разрядов
    DAC     AD5531_1    12 разрядов
    DAC     AD5531_2    12 разрядов
    Регистр. упр.       32 разрядов
*/


PageChannelS *PageChannelS::self = nullptr;


PageChannelS::PageChannelS(wxNotebook *parent) :
    PageChip(parent, "Канал S")
{
    self = this;

    {
        RegAD5531 *dac4 = new RegAD5531(this, dacs[4], "");

        std::vector<StructDescription> desc4;
        desc4.emplace_back(StructDescription{ 0, dac4->GetChip()->BitDepth(), "", "", { true } });
        dac4->SetDescriptionBits(0, desc4);

        AppendRegister(dac4);
    }

    {
        RegAD5531 *dac5 = new RegAD5531(this, dacs[5], "");

        std::vector<StructDescription> desc5;
        desc5.emplace_back(StructDescription{ 0, dac5->GetChip()->BitDepth(), "", "", { true } });
        dac5->SetDescriptionBits(0, desc5);

        AppendRegister(dac5);
    }

    RegFPGA *reg4 = new RegFPGA(this, regs[4]);

    AppendRegister(reg4);
}
