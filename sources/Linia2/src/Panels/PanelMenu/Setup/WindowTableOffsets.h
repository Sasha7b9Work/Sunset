// 2025/7/27 21:36:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"
#include "IPPP/Tests/Ranges.h"
#include "Controls/ButtonsCombo.h"


// Настройки - Таблица смещений и коэффициентов


class WindowTableOffsets : public Dialog
{
public:

    static const int WIDTH = 660;
    static const int HEIGHT = 600;

    WindowTableOffsets();

    virtual ~WindowTableOffsets() { }

private:

    wxRadioButton *rbChanC_MeasI = nullptr,
        *rbChanC_MeasU = nullptr,
        *rbChanB_MeasI = nullptr,
        *rbChanB_MeasU = nullptr,
        *rbChanB_SourceI = nullptr,
        *rbChanB_SourceU = nullptr,
        *rbChanB_LimitI = nullptr,
        *rbChanB_LimitU = nullptr,
        *rbChanS_MeasI = nullptr,
        *rbChanS_MeasU = nullptr,
        *rbChanS_SourceI = nullptr,
        *rbChanS_SourceU = nullptr,
        *rbChanS_LimitI = nullptr,
        *rbChanS_LimitU = nullptr;

    ButtonsCombo *comboType = nullptr;

    struct Field
    {
        wxStaticText *name;
        wxTextCtrl   *value;
        int           range;
    };

    wxVector<Field> fields_I;
    wxVector<Field> fields_U;

    void CreateFields();
    void CreateFields_U(wxPanel *);
    void CreateFields_I(wxPanel *);

    void OnEventButton(wxCommandEvent &);
    void OnEventRadioButton(wxCommandEvent &);
    void OnEventCombobox(wxCommandEvent &);

    // Включить поля смещений
    void FillOffsets(TypeValue::E, bool show_I);

    // Включить поля коэффициентов
    void FillK(TypeValue::E, bool show_I);

    // Заполнить поля в соответствии с текущими установками
    void FillFields();

    void ShowFieldsI(bool);
    void ShowFieldsU(bool);

    // Такое измерение сейчас выбрано чекбоксом
    TypeValue::E CurrentMeasure() const;

    // Выводит значение в стандартизированном для данного окна виде
    wxString GetStringValue(double) const;
};
