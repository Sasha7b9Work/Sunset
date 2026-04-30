// 2026/03/11 14:03:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "System/Events.h"
#include "Panels/PanelMeasures/PanelMeasures.h"
#include "Panels/PanelMeasures/Grid/Grid.h"


void Events::ChangingTheConnectionOfTheMeasuredElement()
{
    ThePanelMeasures->OnEventCnangeMeasuredElement();
}


void Events::ChangeOffsetMeasure(const wxPoint &delta)
{
    TheGrid->OnChangedOffsetMeasure(delta);
}
