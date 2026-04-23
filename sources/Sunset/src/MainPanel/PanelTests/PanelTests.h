// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Controls/BmpButtonsCombo.h"
#include "Controls/Notebook.h"
#include "IPPP/Tests/SettingsTests.h"


class Splitter;
class Panel;


class PanelTests : public PageNotebook
{
public:

    PanelTests(Notebook *, PanelTests *&);

private:

    Splitter *hor_splitter = nullptr;      // Этот сплиттер в горизонтальной плоскости
    Splitter *vert_splitter = nullptr;      // Левый сплиттер (верх/низ)

    Panel *panelLeftTop = nullptr;
    Panel *panelLeftBottom = nullptr;
    Panel *panelRight = nullptr;

    virtual void CallbackOnEventSize() override;
};
 