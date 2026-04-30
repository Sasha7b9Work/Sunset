// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonCombo.h"
#include "Controls/BmpButtonsCombo.h"
#include "Controls/Notebook.h"
#include "IPPP/Tests/SettingsTests.h"


class Splitter;
class Panel;
class PanelCategory;
class PanelLibrary;
class PanelViewTest;


class PageTests : public PageNotebook
{
public:

    PageTests(Notebook *, PageTests *&);

private:

    Splitter *hor_splitter = nullptr;      // Этот сплиттер в горизонтальной плоскости
    Splitter *vert_splitter = nullptr;      // Левый сплиттер (верх/низ)

    PanelCategory *panel_category = nullptr;
    PanelLibrary  *panel_library = nullptr;
    PanelViewTest *panel_view_test = nullptr;

    virtual void CallbackOnEventSize() override;
};
 