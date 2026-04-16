// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Controls/BmpButtonsCombo.h"
#include "Panels/Panels.h"
#include "IPPP/Tests/SettingsTests.h"


struct PanelBoard;


struct PanelTests : public Panel
{
    const int HEIGHT = 500;

    friend struct Category;
    friend struct TypeCommutation;

public:

    PanelTests(PanelBoard *);

    void Pack();
    void Unpack();

private:

    wxButton *btnLoad = nullptr;

    ButtonsCombo *comboTest = nullptr,
        *comboCommutation = nullptr;

    BmpButtonsCombo *comboCategory = nullptr;

    virtual void CallbackOnEventSize() override;

    void OnEventButton(wxCommandEvent &);
    void OnEventComboBox(wxCommandEvent &);
    void OnEventMouseEnter(wxMouseEvent &);
    void OnEventMouseLeave(wxMouseEvent &);
    void OnEventMouseMove(wxMouseEvent &);

    // Нарисовать схему в соответствии с установками
    void BuildPanel();

    // Обновить обозначение типа генерируемой величины в соответствии с выбраной категорией
    void UpdateSuffixGenerator(ButtonsCombo *, char suffix);
};
 