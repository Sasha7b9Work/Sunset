// 2025/6/3 22:47:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Old/PageAD5697.h"
#include "Panels/PanelDebug/PagesBlocks/PanelRegister.h"


PageAD5697 *PageAD5697::self = nullptr;


PageAD5697::PageAD5697(wxNotebook *parent) : PageChip(parent, "AD5697")
{
    self = this;

    wxArrayString names;
    for (int i = 0; i < 4; i++)
    {
        names.push_back("X");
    }
    for (int i = 0; i < 12; i++)
    {
        names.push_back(wxString::Format("D%d", i));
    }
    names.push_back("DAC A");
    names.push_back("0");
    names.push_back("0");
    names.push_back("DAC B");
    for (int i = 0; i < 4; i++)
    {
        names.push_back(wxString::Format("C%d", i));
    }

    PanelRegister *regInput = new PanelRegister(this, "Input Register", 24);

    regInput->SetNamesBits(names);

    std::vector<StructDescription> desc0; //-V827
    desc0.emplace_back(StructDescription{ 0, 8, "DAC DATA" });
    desc0.emplace_back(StructDescription{ 8, 8, "DAC DATA" });
    desc0.emplace_back(StructDescription{ 16, 4, "DAC ADDRESS" });
    desc0.emplace_back(StructDescription{ 20, 4, "COMMAND" });

    regInput->SetDescriptionBits(0, desc0);

    std::vector<StructDescription> desc1; //-V827
    desc1.emplace_back(StructDescription{ 0, 8, "DATA LOW BYTE" });
    desc1.emplace_back(StructDescription{ 8, 8, "DATA HIGH BYTE" });
    desc1.emplace_back(StructDescription{ 16, 8, "COMMAND BYTE" });

    regInput->SetDescriptionBits(1, desc1);

    AppendRegister(regInput);
}
