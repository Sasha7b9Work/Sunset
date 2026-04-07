// 2025/6/1 17:13:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "Panels/ColorSquare.h"


class PanelUpper : public Panel
{
public:

    PanelUpper(wxWindow* parent);

    static PanelUpper *self;

#ifdef WIN32

    wxComboBox *comboPorts = nullptr;

    ColorSquare *color_square = nullptr;

    // Возвращает номер COM-порта из комбобокса
    int GetNumPort() const;

#endif

private:

#ifdef WIN32

    void OnEventComboBox(wxCommandEvent &);

    // Установить в comboPorts порт из настроек
    void SetTunedPort();

#endif
};
