// 2025/08/19 12:59:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelScheme/PainterScheme.h"
#include "IPPP/Tests/Tests.h"
#include "Controls/Bitmap.h"


void PainterScheme::Build()
{
    BeginPaint(colorBackground);

    DrawElement();

    EndPaint();
}


void PainterScheme::DrawElement()
{
    static const pchar files[Category::Count] =
    {
        "sch/BCE_N.bmp",
        "sch/BCE_P.bmp",
        "sch/GDS_N.bmp",
        "sch/GDS_P.bmp",
        "sch/BCSE_N.bmp",
        "sch/BCSE_P.bmp",
        "sch/GDBS_N.bmp",
        "sch/GDBS_P.bmp",
        "sch/diod2.bmp",
        "sch/GASK.bmp",
        "sch/resistor.bmp",
        "sch/capasitor.bmp"
    };

    DrawBitmap(50, 33, Bitmap::Get(files[Category::Current()]).GetBitmap(), {241, 241, 241});
}
