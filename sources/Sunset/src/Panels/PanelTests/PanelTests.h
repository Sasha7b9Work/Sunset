// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Controls/BmpButtonsCombo.h"
#include "Panels/Panels.h"
#include "IPPP/Tests/SettingsTests.h"


class PanelBoard;
class Splitter;


class PanelTests : public Panel
{
public:

    PanelTests(PanelBoard *, PanelTests *&);

private:

    Splitter *hor_splitter = nullptr;      // Этот сплиттер в горизонтальной плоскости
    Splitter *vert_splitter = nullptr;      // Левый сплиттер (верх/низ)

    wxPanel *panelLeftTop = nullptr;
    wxPanel *panelLeftBottom = nullptr;
    wxPanel *panelRight = nullptr;

    virtual void CallbackOnEventSize() override;
};
 