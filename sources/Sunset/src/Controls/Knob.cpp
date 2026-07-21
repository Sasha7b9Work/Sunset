// 2026/07/21 14:44:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Knob.h"


void KnobWidget::OnMouseMove(wxMouseEvent &event)
{
    if (dragging && event.LeftIsDown())
    {
        // ВАЖНО: Возвращаем курсор мыши в исходную позицию
        // Это создает эффект, что курсор остается на месте при вращении
        WarpPointer(capturePoint.x, capturePoint.y);

        wxPoint currentPos = event.GetPosition();

        static float delta = 0.0f;

        // Вычисляем дельту относительно точки захвата
        float deltaY = (float)(currentPos.y - capturePoint.y) / 50.0f;
        float deltaX = (float)(-currentPos.x + capturePoint.x) / 50.0f;

        delta += (deltaX + deltaY);

        if ((int)delta != 0)
        {
            // Изменяем значение в зависимости от направления движения
            // Тянем вверх (отрицательный deltaY) - увеличиваем значение
            // Тянем вниз (положительный deltaY) - уменьшаем значение
            int newValue = (int)((float)value - (float)((int)delta)); // Минус потому что Y увеличивается вниз

            // Ограничиваем значение
            if (newValue < minValue) newValue = minValue;
            if (newValue > maxValue) newValue = maxValue;

            SetValue(newValue);

            delta -= (float)((int)delta);
        }
    }

    event.Skip();
}