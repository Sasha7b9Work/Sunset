// 2024/11/02 18:44:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace SD
{
    // Расстояние между статикбоксами по вертикали
    int DSBY();

    // Координаты первого элемента в комбобоксе
    wxPoint XY0();

    // Размеры - dimensions
    namespace D
    {
        // ChooseSoundDialog
        namespace ChooseSound
        {
            wxSize SizeSpin();
            wxPoint PosSpin(int x, int y);
            wxSize SizeCombo();
            wxPoint PosCombo(int x, int y);
            int d();
            int y();
            wxSize Size();
        }

        namespace CommonPanel
        {
            int Delta();
        }

        namespace ColorDialog
        {
            wxSize Size();
            wxSize SizeSlider();
            int Spacer();
        }
    }
}
