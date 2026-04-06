// 2025/08/30 11:32:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageAD5300.h"


PageAD5300 *PageAD5300::self = nullptr;


PageAD5300::PageAD5300(wxNotebook *parent) : PageChip(parent, "AD5300")
{
    self = this;

    wxArrayString names;
    for (int i = 0; i < 4; i++)
    {
        names.push_back("X");
    }
    for (int i = 0; i < 8; i++)
    {
        names.push_back(wxString::Format("D%d", i));
    }
    names.push_back("PD0");
    names.push_back("PD1");
    for (int i = 0; i < 2; i++)
    {
        names.push_back("X");
    }

    PanelRegister *regInput = new PanelRegister(this, "Input Register", 16);

    regInput->SetNamesBits(names);

    std::vector<StructDescription::CommandStruct> commands; //-V826 //-V827
    commands.emplace_back(StructDescription::CommandStruct{ 0, "Normal operation" });
    commands.emplace_back(StructDescription::CommandStruct{ 1, "1 kOhm to GND" });
    commands.emplace_back(StructDescription::CommandStruct{ 2, "100 kOhm to GND" });
    commands.emplace_back(StructDescription::CommandStruct{ 3, "Three-State" });

    std::vector<StructDescription> desc0; //-V827
    desc0.emplace_back(StructDescription{  0, 4, "not used" });
    desc0.emplace_back(StructDescription{  4, 8, "DATA BITS", { true } });
    desc0.emplace_back(StructDescription{ 12, 2, "POWER_DOWN MODES", { false, true, commands} });
    desc0.emplace_back(StructDescription{ 14, 2, "not used" });

    regInput->SetDescriptionBits(0, desc0);

    AppendRegister(regInput);
}
