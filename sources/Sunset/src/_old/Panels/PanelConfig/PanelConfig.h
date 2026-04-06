// 2025/6/1 17:20:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "Controls/ButtonsCombo.h"


class PanelConfig : public Panel
{
    friend class PanelScheme;
    friend class MainWindow;

public:

    static const int HEIGHT = 500;
    static const int HEIGHT_BUTTONS = 40;   // Такую высоту занимают кнопки
    static const int WIDTH_COMBO = 180;
    static const int DYC = 26;              // Расстояние между комбобоксами по вертикали
    static const int X = 6;

    void Pack();

    void Unpack();

    void EnableSubPanels(bool);

private:

    PanelConfig(wxWindow *parent);

    wxToggleButton *btnScheme = nullptr,
        *btnChannelC = nullptr,
        *btnChannelB = nullptr,
        *btnChannelS = nullptr,
        *btnModel = nullptr,
        *btnCalc = nullptr;

    // Структура хранит кнопку и панель, которую та включает
    struct StructPanel
    {
        wxToggleButton *button;
        wxPanel        *panel;
    };

    // Здесь хранятся элементы панели "Канал C"
    struct ChannelC
    {
        std::vector<wxRadioButton *> rbs;
    };

    std::vector<StructPanel> str_panels;

    wxPanel *CreatePanel(wxToggleButton *);

    void OnEventToggleButton(wxCommandEvent &);

    // Отщёлкнуть все, кроме id
    void UnсheckAllAcross(int id);

    void EnablePanel(int button_id);

    wxPanel *GetCurrentPanel();

    void SetCurrentPanel(const wxString &name);
};
