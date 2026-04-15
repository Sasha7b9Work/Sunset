// 2025/6/4 15:46:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"
#include "Panels/PanelDebug/Notebook/AnimatedImpulse.h"


class Register;


class PainterRegister : public wxPanel
{
public:

    // Ширина бита
    static const int W_B = 20;

    PainterRegister(wxWindow *parent, Register *panel, const wxPoint &position);

    wxFont GetDefaultFont(int size = -1);

    // Возвращает координату X бита num_bit
    int BitX(int num_bit, int all_bits) const;

    // Увеличить высоту на dH
    void IncreaseHeight(int dH);

    AnimatedImpulse *animation = nullptr;

private:

    bool             first_paint = true;
    Register        *panel = nullptr;

    // Возвращает координаты верхнего левого угла отображения бита
    wxPoint CoordBit(int num_bit);

    void DrawTitleBit(int num_bit, const wxString &title, wxGraphicsContext *);

    void DrawDescriptions(int index, wxGraphicsContext *);

    // Если нельзя вписать текст в данную ширину, то размер шрифта будет уменьшаться, пока не впишется
    void DrawTextInCenter(int x, int y, int width, const wxString &, int size, wxGraphicsContext *);

    void SetHintCheckBox(int num_bit);

    wxString GetHint(int index_desc, int num_bit);

    void OnEventPaint(wxPaintEvent &);

    wxColor ColorBackground(bool enabled) const;

    void SetPositionAnimationWidget();
};
