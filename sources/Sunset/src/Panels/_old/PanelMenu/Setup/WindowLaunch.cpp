// 2025/8/1 21:15:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/Setup/WindowLaunch.h"
#include "Controls/StaticBoxCustom.h"


WindowLaunch::WindowLaunch() :
    Dialog("Запуск", wxDefaultPosition, { WIDTH, HEIGHT })
{
    std::vector<std::unique_ptr<DescriptionControl>> controls;
    controls.emplace_back(new DescRadioButton(&rbOld, "ИППП3", { 10, 20 }, wxRB_GROUP));
    controls.emplace_back(new DescRadioButton(&rbNew, "B1505", { 10, 50 }));

    new StaticBoxCustom(MainWidget(), "Внешний вид", { 10, TopY() + 10 }, { 200, 100 }, controls);

    Bind(wxEVT_RADIOBUTTON, &WindowLaunch::OnEventRadioButton, this);
}


void WindowLaunch::OnEventRadioButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == rbOld->GetId())
    {
        id++;
    }
    else if (id == rbNew->GetId())
    {
        id++;
    }

    (void)id;
}
