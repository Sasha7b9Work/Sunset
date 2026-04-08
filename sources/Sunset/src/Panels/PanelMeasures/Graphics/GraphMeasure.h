// 2025/7/14 17:22:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelMeasures/Grid/Grid.h"


class GraphMeasure
{
public:

    void AppendPoint(const wxPoint2DDouble &);

    void Draw();

    void SetColor(const wxColor &_color)
    {
        color = _color;
    }

    static void CreateForEmulator(std::vector<GraphMeasure *> &);

    std::vector<wxPoint> rel_points;            // А здесь уже значения в координатах экрана, готовые к отрисовке

private:

    std::vector<wxPoint2DDouble> abs_points;    // В абсолютных значениях - вольты, амперы

    wxColor color{ *wxWHITE };
};
