// 2025/6/4 10:25:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
//#include "Panels/PanelDebug/Notebook/PainterRegister.h"
#include "Controls/TextControls.h"
//#include "Panels/PanelDebug/Notebook/CommandsCombo.h"
#include "Controls/Painter.h"
#include "IPPP/Real/Chips.h"
#include "Controls/Knob.h"
#include "Controls/Slider.h"


class CommandsCombo;


// Квадратик с 0 или 1, на который щёлкают ЛКМ и он изменяет своё состояние
class CheckBoxBit : public Painter
{
public:
    CheckBoxBit(wxWindow *, const wxPoint &, const wxSize &);

    void SetValue(bool);

    bool IsChecked() const;

    virtual bool Enable(bool) override;

private:

    bool value = false;

    void RePaint();

    void OnEventLeftClick(wxMouseEvent &);
};


// Описание группы бит
struct StructDescription
{
    struct CommandStruct
    {
        uint16   value;  // Значение
        wxString desc;   // Описание данного значения
        wxString CreateFullLine(StructDescription &) const;
        wxString CreateTooltip(StructDescription &) const;
    };
    int      first_bit;
    int      num_bits;
    wxString desc;
    wxString hint;
    struct DecField
    {
        bool need_text_ctrl_dec = false;            // если true, то есть возможность вводить десятичное значение
        std::vector<CommandStruct> commands;        // Сюда ложим нужные команды, чтобы потом создать combo
        TextCtrlNumber *text_ctrl_dec = nullptr;    // Здесь находятся десятичные значения. Если nullptr - его нету
        CommandsCombo *combo = nullptr;             // А здесь находятся команды
    } field;

    // Рассчитать значение в соотвествие с чекбоксами
    uint CalculateValue(std::vector<CheckBoxBit *> &);
};


struct StateBit
{
    int num;        // Номер бита
    bool state;     // Состояние, в которое должен быть установлен бит
};


// Описание режима - при его выборе заданные биты устанавливаются в заданные положения
struct ModeDescripion
{
    wxString name;                  // Это название будет на кнопки органа управления
    wxString hint;                  // Более развёрнутое описание - будет выведено при наведении мыши на орган управления
    std::vector<StateBit> state;    // При выборе данного режима биты будут установлены в данные состояния
};


// Визуальное представление регистра

class Register : public wxPanel
{
    friend class PainterRegister;

public:

    static const int WIDTH = 800;
    static const int HEIGHT = 155;

    Register(wxWindow *parent, const wxString &_title,      // Это написано на изображении
        Chip *, const wxString &_functional);

    void SetNamesBits(const wxArrayString &);

    void SetDescriptionBits(int index, const std::vector<StructDescription> &);

    void AppendModes(const wxString &title, const std::vector<ModeDescripion> &);

    void SetValue(uint);
    // Занести значения в определённые биты
    void SetValueToBits(uint, int first_bit, int num_bits);
    uint GetValueFromBits(int first_bit, int num_bits) const;
    void Pack();
    virtual void Unpack();

    // Записать значение в аппаратуру
    void WriteValue();

    const Chip *GetChip() const
    {
        return chip;
    }

protected:

    Chip *chip = nullptr;
    PainterRegister *painter = nullptr;

    Button *btnSend = nullptr;                  // Однократная засылка
    ToggleButton *btnAutoSend = nullptr;        // Если кнопка нажата, то каждую секунду происходит запись в данный регистр
    wxTimer timerAutoSend;                      // По этому таймеру будут автозасылки

    std::vector<wxWindow *> windows;

    wxArrayString names_bits;                   // Названия битов

    std::vector<StructDescription> desc[2];     // Описания групп битов

    static const uint NUM_MODES = 5;
    std::vector<ModeDescripion> modes[NUM_MODES];           // Описания режимов
    wxString title_modes[5];                                // Названия режимов
    // А это элемент управления для выбора режимов
    CommandsCombo *combo_modes[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };

    std::vector<CheckBoxBit *> chboxes;

    // Создать элемент управления для выбора режима
    void CreateControlMode(int i);

    uint GetValue() const;

    // Возвращает true, если нужно выводить текстовое поле для десятичного значения хотя бы у одной группы бит
    bool NeedTextCtrlDEC() const;

    // Привести десятичные поля в соответствие с битовыми
    void UpdateDecFields();

    // Привести комбобоксы команд и режимов в соотвествие с битовыми полями
    void UpdateComboCommandsAndModes();

    // Увеличить высоту на dH
    void IncreaseHeight(int dH);

    void OnEventTextCtrl(wxCommandEvent &);
    // ЛКМ по состоянию бита CheckBoxBit
    void OnEventCheckBoxBit(wxCommandEvent &);
    // Управление состоянием групп битов
    void OnEventComboField(wxCommandEvent &);
    // Выбор режима
    void OnEventComboMode(wxCommandEvent &);
    void OnEventToggleButton(wxCommandEvent &);
    void OnEventButton(wxCommandEvent &);
    void OnEventTimerAutoSend(wxTimerEvent &);

    // Все элементы кроме wnd будут установлены в состояние active
    void SetActiveAcross(bool active, wxWindow *wnd);

    virtual void OnEventUpdateComboCommandsAndModes() = 0;
};


class RegFPGA : public Register
{
public:

    RegFPGA(wxWindow *_parent, Chip *_chip) :
        Register(_parent, "", _chip, "")
    {
    }

    virtual void OnEventUpdateComboCommandsAndModes() override { }
};


class RegDAC : public Register
{
public:
    RegDAC(wxWindow *, pchar _title, Chip *_chip, const wxString &_functional);

protected:

    // Столько бит занимает поле со значением
    int NumBitsValue() const;

private:

    KnobWidget *knob = nullptr;         // Ручка установки значения
    SliderInt *slider = nullptr;        // Ползунок установки значения

    void OnEventKnob(wxCommandEvent &);
    void OnEventSlider(wxCommandEvent &);

    // Первый бит поля, где хранится значение
    int FirstBitValue() const;

    uint GetValueDAC();
    void SetValueDAC(uint);

    virtual void Unpack() override;

    void SetValueToKnobAndSlider();

    virtual void OnEventUpdateComboCommandsAndModes() override;

    void OnEventChangedControl(int);
};


class RegAD5443 : public RegDAC
{
public:

    // Если need_knob, то рисуем ручку
    RegAD5443(wxWindow *, Chip *_chip, const wxString &_functional);
};


class RegAD5531 : public RegDAC
{
public:

    RegAD5531(wxWindow *, Chip *_chip, const wxString &_functional);
};
