// 2026/04/08 15:17:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Panels/Panels.h"


struct PanelTables : public Panel
{
    PanelTables(wxWindow *);

private:

    virtual void CallbackOnEventSize() override;
};
