// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Panels/PanelTests/PainterScheme.h"
#include "Controls/BmpButtonsCombo.h"
#include "Panels/Panels.h"
#include "IPPP/Tests/SettingsTests.h"


// Отображается на отрисованной схеме
class ComboJack : public ButtonsCombo
{
public:
    ComboJack(Chan::E, wxWindow *parent, const wxString &title, int width, const wxArrayString &labels);
    // Настроить текущее состояние в зависимости от текущих установок
    void TuneState();
private:
    Chan::E channel;
    // Установить видимость или невидимость в зависимости от текущих установок
    void SetVisibility();

    void SetChoices();

    void SetChoice(StateJack::E);
};


// На панели "Коммутация" объединяет изображение гнезда и комбобокса с вариантами
class FullJack : public wxPanel
{
public:
    FullJack(Chan::E, wxWindow *parent, pchar file_jack_bmp, const wxArrayString * = nullptr);
    // Настроить текущее состояние в зависимости от текущих установок
    void TuneState();

    void Pack();
    void Unpack();
private:
    ButtonsCombo *combo = nullptr;
    PainterBMP *painterBMP = nullptr;
    Chan::E channel = Chan::Count;
    // Установить видимость или невидимость в зависимости от текущих установок
    void SetVisibility();

    void SetChoices();

    void SetChoice(StateJack::E);
};


struct PanelTests : public Panel
{
    const int HEIGHT = 500;

    friend struct Category;
    friend struct TypeCommutation;

public:

    PanelTests(wxWindow *);

    void Pack();
    void Unpack();

private:

    wxButton *btnLoad = nullptr;

    ButtonsCombo *comboTest = nullptr,
        *comboCommutation = nullptr;

    ComboJack *combos[Chan::Count];         // Комбобоксы на схеме. Активны при внешней коммутации
    FullJack *jacks[Chan::Count];           // Активны при внутренней коммутации

    BmpButtonsCombo *comboCategory = nullptr;

    PainterScheme *painter = nullptr;                 // Здесь будет нарисована схема


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
 