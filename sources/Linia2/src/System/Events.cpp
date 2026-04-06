// 2026/03/11 14:03:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "System/Events.h"
#include "Display/Display.h"
#include "Display/Grid/Grid.h"


void Events::ChangingTheConnectionOfTheMeasuredElement()
{
    TheDisplay->OnEventCnangeMeasuredElement();
}


void Events::ChangeOffsetMeasure(const wxPoint &delta)
{
    TheGrid->OnChangedOffsetMeasure(delta);
}
