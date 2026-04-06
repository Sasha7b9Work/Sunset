// 2025/7/27 19:54:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/Setup/WindowCalibrate.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/SystemDepend.h"
#include "Controls/StaticBox.h"


WindowCalibrate::WindowCalibrate() :
    Dialog(wxT("Калибровка"), wxDefaultPosition, {WIDTH, HEIGHT})
{
    int d = 20;

    int y = 20;

    new wxStaticText(this, wxID_ANY, wxT("Серийный номер прибора"), { d, y });
    textSerialNumber = new wxStaticText(this, wxID_ANY, "000", { 200, y });

    y = 50;

    new wxStaticText(this, wxID_ANY, wxT("Дата последней калибровки"), { d, y });
    textDateTime = new wxStaticText(this, wxID_ANY, "", { 200, y });

    textDateTime->SetLabel(wxDateTime::Now().Format("%d.%m.%Y"));

    new wxButton(this, wxID_ANY, wxT("Сохранить"), { 300, 30 }, { 75, BUTTON_HEIGHT });

    StaticBox *box = new StaticBox(this, wxT("Параметры режима калибровки"), { d, y + 30 }, { 270, 330 });

    {
        y = d + 10;

        wxArrayString choices;
        choices.push_back("50A");

        comboRange = new ButtonsComboRange(box, wxT("Диапазон"), { 0, SD::Y_SB(y) }, 150, choices, choices, "comboRange");

        y += d;

        y = CreateLabelGroup(box, d, y, wxT("Канал C"));
        y = CreateRadioButton(box, d, y,  &rbChanC_MeasI, wxT("Измеритель I"));
        rbChanC_MeasI->SetFocus();
        y = CreateRadioButton(box, d, y, &rbChanC_MeasU, wxT("Измеритель U"));

        y = CreateLabelGroup(box, d, y, wxT("Канал B"));
        y = CreateRadioButton(box, d, y, &rbChanB_MeasI, wxT("Источник I / Измеритель I"));
        y = CreateRadioButton(box, d, y, &rbChanB_MeasU, wxT("Источник U / Измеритель U"));

        y = CreateLabelGroup(box, d, y, wxT("Канал S"));
        y = CreateRadioButton(box, d, y, &rbChanS_MeasI, wxT("Источник I / Измеритель I"));
        y = CreateRadioButton(box, d, y, &rbChanS_MeasU, wxT("Источник U / Измеритель U"));
    }

    y = 430;

    box = new StaticBox(this, wxT("Внимание!"), { d, y }, { 300, 100 });

    new wxStaticText(box, wxID_ANY, _("Соберите схему для определения основной погрешности измерения ") +
        _("тока по каналу C на диапазонах от 5 A до 50 A (рис. В.8 РЭ) и установите резистор Rn"),
        { d, SD::Y_SB(d) }, { 260, 75 });

    y += box->GetSize().y;

    wxSize size{ 70, BUTTON_HEIGHT };

    new wxButton(this, wxID_ANY, wxT("Пуск"), { 50, y + 30 }, size);
    new wxButton(this, wxID_ANY, wxT("Отмена"), { 200, y + 30 }, size);

    Bind(wxEVT_BUTTON, &WindowCalibrate::OnEventButton, this);
}


void WindowCalibrate::OnEventButton(wxCommandEvent &)
{

}
