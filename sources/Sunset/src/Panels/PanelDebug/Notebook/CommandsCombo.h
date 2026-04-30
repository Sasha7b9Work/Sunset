// 2025/08/30 16:03:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"


class CommandsCombo : public ButtonsCombo
{
public:

    CommandsCombo(wxWindow *parent, const wxString &title, int width,
        const wxArrayString &labels, const wxArrayString &tooltips,
        const wxString &name);

    // Если в комбобоксе нету выбора для установленных значений битов, надо вызывать эту функцию
    void SetInvalidChoice();

    virtual void SetMyToolTip(const wxString &) override;

private:

    virtual void SetExtendedLabel(const wxString &, const wxString &) override;
};
