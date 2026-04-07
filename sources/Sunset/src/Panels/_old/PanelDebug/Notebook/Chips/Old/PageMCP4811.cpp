// 2025/6/3 22:32:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Old/PageMCP4811.h"


PageMCP4811 *PageMCP4811::self = nullptr;


PageMCP4811::PageMCP4811(wxNotebook *parent) : PageChip(parent, "MCP4811")
{
    self = this;

    wxArrayString names;

    {
        for (int i = 0; i < 2; i++)
        {
            names.push_back("X");
        }

        for (int i = 0; i < 10; i++)
        {
            names.push_back(wxString::Format("D%d", i));
        }

        names.push_back("SHDN");
        names.push_back("GA");
        names.push_back("-");
        names.push_back("0");

        PanelRegister *reg5_2 = new PanelRegister(this, "Register 5-2", 16);

        reg5_2->SetNamesBits(names);

        AppendRegister(reg5_2);
    }
}
