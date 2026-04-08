// 2025/7/14 17:22:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDisplay/Graphics/GraphMeasure.h"
#include "Panels/PanelDisplay/PanelMeasures.h"
#include "Settings/Settings.h"


void GraphMeasure::AppendPoint(const wxPoint2DDouble &point)
{
    abs_points.push_back(point);
}


void GraphMeasure::Draw()
{
    ThePanelDisplay->gc->SetBrush(color);
    ThePanelDisplay->gc->SetPen(color);

    rel_points.clear();

    wxPoint2DDouble range_x = TheGrid->GetRangeX();
    wxPoint2DDouble range_y = TheGrid->GetRangeY();

    bool very_big = false;

    for (auto &coord : abs_points)
    {
        if(coord.m_x > range_x.m_x && coord.m_x < range_x.m_y &&
           coord.m_y > range_y.m_x && coord.m_y < range_y.m_y)
        {
        }
        else
        {
            very_big = true;
        }

        wxPoint point = TheGrid->ValuesToCoord(coord.m_x, coord.m_y);

        rel_points.push_back(point);
    }

    Spline().Draw(rel_points, very_big ? false : true, true);

    ThePanelDisplay->LoadColors();
}


void GraphMeasure::CreateForEmulator(std::vector<GraphMeasure *> &entities)
{
    for (int num_line = 0; num_line < 10; num_line++)
    {
        GraphMeasure *meas = new GraphMeasure();

        meas->SetColor(SET::GUI::color_curve->Get());

        for (double u = 0.0; u <= 1.0; u += 0.1) //-V1034
        {
            // Базовый ток пропорционален номеру линии (от 5 до 50 мкА)
            double Ib = 5.0 + (num_line + 1 - 1) * 5.0; // мкА

            // Коэффициент усиления
            double beta = 8000.0 + (num_line + 1) * 1000.0; // немного растет с током

            // Параметры формы кривой
            double U_nas = 30.0;                // напряжение насыщения
            double smoothness = 150.0f;         // плавность перехода
            double Early = 300.0;               // напряжение Эрли

            // Максимальный ток коллектора
            double Ik_max = beta * Ib * 1e-6;

            // Плавный переход от 0 к Ik_max с помощью tanh
            double transition = tanh(smoothness * u / U_nas);

            // Эффект Эрли - небольшой наклон кривых
            double early_effect = 1.0 + u / Early;

            // Небольшая кривизна для реалистичности
            double curvature = 1.0 - 0.05 * exp(-u * 10.0);

            double i = Ik_max * transition * early_effect * curvature;

            meas->AppendPoint({ u, i });
        }

        entities.push_back(meas);
    }
}
