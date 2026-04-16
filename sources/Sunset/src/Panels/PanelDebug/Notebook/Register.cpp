// 2025/6/4 10:27:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/Register.h"
#include "Panels/PanelDebug/Notebook/PainterRegister.h"
#include "Controls/TextControls.h"
#include "Utils/StringUtils.h"
#include "Panels/PanelDebug/Notebook/CommandsCombo.h"
#include "IPPP/IDevice.h"
#include "Panels/PanelDebug/Notebook/NotebookDebug.h"
#include "Panels/PanelDebug/Notebook/PageChip.h"
#include "Controls/Knob.h"
#pragma warning(push, 0)
#include <wx/stattext.h>
#pragma warning(pop)


// Функции для комбобоксов выбора режимов
namespace ComboRange
{
    static void UpdateState(std::vector<ModeDescripion> &, const std::vector<CheckBoxBit *> &chbox, CommandsCombo *);
}


Register::Register(wxWindow *parent, const wxString &_title, Chip *_chip, const wxString &_functional) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { WIDTH, HEIGHT }, wxTAB_TRAVERSAL | wxSIMPLE_BORDER),
    chip(_chip)
{
    wxWindowBase::SetBackgroundColour(parent->GetBackgroundColour().ChangeLightness(110));

    wxPanel::SetName("Register");

    new wxStaticText(this, wxID_ANY, _title + " " + chip->GetNameDevice() + (_functional.IsEmpty() ? wxString("") : (wxString(" : ") + _functional)), {10, 10});

    wxSize size_button{ 90, 25 };

    int x = 690;

    btnSend = new Button(this, wxT("Записать"), size_button);
    btnSend->SetToolTip(L("Однократная засылка в регистр"));
    btnSend->SetPosition({ x, 0 });
    btnSend->Bind(wxEVT_BUTTON, &Register::OnEventButton, this);

    windows.push_back(btnSend);

    x -= size_button.x + 5;

    btnAutoSend = new ToggleButton(this, wxT("Автозапись"), size_button);
    btnAutoSend->SetToolTip(L("Автоматическая засылка в регистр 1 раз в секунду"));
    btnAutoSend->SetPosition({ x, 0 });
    btnAutoSend->Bind(wxEVT_TOGGLEBUTTON, &Register::OnEventToggleButton, this);

    windows.push_back(btnAutoSend);

    x -= size_button.x + 5;

    int y0 = 40;

    {
        painter = new PainterRegister(this, this, { 10, y0 });

        for (auto box : chboxes)
        {
            box->Bind(wxEVT_CHECKBOX, &Register::OnEventCheckBoxBit, this);
        }
    }

    Bind(wxEVT_RIGHT_DOWN, [](wxMouseEvent &event)
        {
            event.Skip(); // Пропустить событие дальше (родителю)
        });

    wxArrayString names;
    for (int i = 0; i < chip->BitDepth(); i++)
    {
        names.push_back(wxString::Format("D%d", i));
    }

    SetNamesBits(names);

    Bind(wxEVT_TIMER, &Register::OnEventTimerAutoSend, this);

    timerAutoSend.SetOwner(this, timerAutoSend.GetId());
}


void Register::SetNamesBits(const wxArrayString &_names)
{
    names_bits = _names;
}


void Register::AppendModes(const wxString &title, const std::vector<ModeDescripion> &mode_desc)
{
    for (uint i = 0; i < NUM_MODES; i++)
    {
        if (modes[i].size() == 0)
        {
            modes[i] = mode_desc;
            title_modes[i] = title;
            CreateControlMode((int)i);
            break;
        }
    }

    int dH = 30;

    painter->IncreaseHeight(dH);

    IncreaseHeight(dH);

    PageChip *page_chip = (PageChip *)GetParent();

    page_chip->Rebuild();
}


void Register::IncreaseHeight(int dH)
{
    wxSize size = GetSize();
    size.y += dH;

    SetMinSize(size);
    SetMaxSize(size);

    SetSize(size);

    Layout();
}


void Register::SetDescriptionBits(int index, const std::vector<StructDescription> &_desc)
{
    desc[index] = _desc;

    if (index == 0)
    {
        bool need_dec = NeedTextCtrlDEC();

        for (auto &elem : desc[0])
        {
            if (elem.field.need_text_ctrl_dec)
            {
                int num_bit = elem.first_bit + elem.num_bits - 1;

                int x = painter->BitX(num_bit, chip->BitDepth());

                int num_y = elem.desc[0] ? 3 : 2;

                elem.field.text_ctrl_dec = new TextCtrlNumber(painter, wxID_ANY, "",
                    { PainterRegister::W_B * elem.num_bits + 1, 20 },
                    0, (1 << elem.num_bits) - 1);
                elem.field.text_ctrl_dec->SetPosition({ x, (PainterRegister::W_B + 1) * num_y });

                elem.field.text_ctrl_dec->Bind(wxEVT_TEXT, &Register::OnEventTextCtrl, this);
            }

            if (elem.field.commands.size())
            {
                int num_bit = elem.first_bit + elem.num_bits - 1;

                int x = painter->BitX(num_bit, chip->BitDepth()) + 1;

                wxArrayString names;
                for (auto &com : elem.field.commands)
                {
                    names.push_back(com.CreateFullLine(elem));
                }

                wxArrayString tooltips;
                for (auto &com : elem.field.commands)
                {
                    tooltips.push_back(com.CreateTooltip(elem));
                }

                int y = (PainterRegister::W_B + 1) * (need_dec ? 4 : 3) - 1;

                elem.field.combo = new CommandsCombo(painter, elem.hint,
                   PainterRegister::W_B * elem.num_bits - 1, names, tooltips, "Register");
                elem.field.combo->SetPosition({ x, y });

                elem.field.combo->left_align = true;

                elem.field.combo->Bind(wxEVT_COMBOBOX, &Register::OnEventComboField, this);
            }
        }
    }

    UpdateDecFields();
}


void Register::CreateControlMode(int i)
{
    int x = 10 + i * 300;
    int y = 110;

    new wxStaticText(painter, wxID_ANY, title_modes[i], { x, y });

    wxArrayString names;
    for (auto &mode : modes[i])
    {
        names.push_back(mode.name);
    }

    wxArrayString tooltips;
    for (auto &mode : modes[i])
    {
        tooltips.push_back(mode.hint);
    }

    combo_modes[i] = new CommandsCombo(painter, title_modes[i], 250, names, tooltips, title_modes[i]);
    combo_modes[i]->SetPosition({ x, y + 25 });

    combo_modes[i]->Bind(wxEVT_COMBOBOX, &Register::OnEventComboMode, this);
}


void Register::OnEventTextCtrl(wxCommandEvent &event)
{
    int id = event.GetId();

    for (auto &d : desc[0])
    {
        if (d.field.need_text_ctrl_dec)
        {
            if (id == d.field.text_ctrl_dec->GetId())
            {
                wxString str = d.field.text_ctrl_dec->GetValue();

                long value = 0;
                str.ToLong(&value);

                int index = d.first_bit;        // Индекс первого бита (младшего)

                for (int i = 0; i < d.num_bits; i++)
                {
                    bool bit = value & 1;

                    chboxes[(uint)index++]->SetValue(bit);

                    value >>= 1;
                }

                break;
            }
        }
    }

    UpdateComboCommandsAndModes();

    event.Skip();
}


void Register::OnEventToggleButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (btnAutoSend && id == btnAutoSend->GetId())
    {
        SetActiveAcross(event.GetInt() == 0, btnAutoSend);

        if (event.GetInt())
        {
            timerAutoSend.Start(1000);
            painter->animation->RunPeriodic();
        }
        else
        {
            timerAutoSend.Stop();
            painter->animation->Stop();
        }

        WriteValue();
    }

    event.Skip();
}


void Register::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnSend->GetId())
    {
        WriteValue();
        painter->animation->RunOnce();
    }
}


void Register::WriteValue()
{
    chip->WriteValue(GetValue());
}


void Register::OnEventTimerAutoSend(wxTimerEvent &event)
{
    WriteValue();

    event.Skip();
}


void Register::SetActiveAcross(bool active, wxWindow *_wnd)
{
    for (auto wnd : windows)
    {
        if (wnd != _wnd)
        {
            wnd->Enable(active);
        }
    }

    TheNotebookDebug->EnableSwitching(active);
}


bool Register::NeedTextCtrlDEC() const
{
    for (uint num_desc = 0; num_desc < 2; num_desc++)
    {
        const std::vector<StructDescription> &d = desc[num_desc];

        if (d.size() > 0)
        {
            for (uint i = 0; i < d.size(); i++)
            {
                if (d[i].field.need_text_ctrl_dec)
                {
                    return true;
                }
            }
        }
    }

    return false;
}


void Register::UpdateDecFields()
{
    if (!NeedTextCtrlDEC())
    {
        return;
    }

    for (int i = 0; i < (int)chboxes.size(); i++)                       // Перебираем все биты
    {
        for (auto &d : desc[0])
        {
            if (d.field.need_text_ctrl_dec)
            {
                if(i == d.first_bit)                                    // Нашли первый бит поля
                {
                    uint value = d.CalculateValue(chboxes);

                    d.field.text_ctrl_dec->SetValue(wxString::Format("%u", value));

                    i += d.num_bits;                                    // Переходим к следующему полю
                }
            }
        }
    }
}


uint StructDescription::CalculateValue(std::vector<CheckBoxBit *> &chbox)
{
    uint value = 0;

    int counter = 0;

    for (int bit = first_bit; bit < first_bit + num_bits; bit++)
    {
        if (chbox[(uint)bit]->IsChecked())
        {
            value |= (1 << counter);
        }

        counter++;
    }

    return value;
}


void Register::OnEventCheckBoxBit(wxCommandEvent &event)
{
    UpdateDecFields();

    UpdateComboCommandsAndModes();

    event.Skip();
}


wxString StructDescription::CommandStruct::CreateFullLine(StructDescription &d) const
{
    return SU::BinToString(value, d.num_bits) + " - " + desc;
}


wxString StructDescription::CommandStruct::CreateTooltip(StructDescription &) const
{
    return desc;
}


void Register::OnEventComboField(wxCommandEvent &event)
{
    int id = event.GetId();

    for (auto &d : desc[0])
    {
        if (d.field.commands.size())
        {
            if (d.field.combo->GetId() == id)
            {
                int num_bit = d.first_bit;
                uint16 value = d.field.commands[(uint)event.GetInt()].value;

                for (int i = 0; i < d.num_bits; i++)
                {
                    chboxes[(uint)num_bit++]->SetValue((value & (1 << i)) != 0);
                }

                break;
            }
        }
    }

    UpdateDecFields();

    event.Skip();
}


void Register::OnEventComboMode(wxCommandEvent &event)
{
    int id = event.GetId();

    int num_mode = -1;

    for (int i = 0; i < 5; i++)
    {
        if (combo_modes[i])
        {
            if (combo_modes[i]->GetId() == id)
            {
                num_mode = i;
                break;
            }
        }
        else
        {
            break;
        }
    }

    if (num_mode == -1)
    {
        return;
    }

    ModeDescripion &mode = modes[num_mode][(uint)event.GetInt()];

    for (uint i = 0; i < mode.state.size(); i++)
    {
        StateBit &state = mode.state[i];

        chboxes[(uint)state.num]->SetValue(state.state);
    }

    UpdateComboCommandsAndModes();
}


void Register::UpdateComboCommandsAndModes()
{
    for (auto &d : desc[0])
    {
        if (d.field.commands.size())
        {
            uint value = d.CalculateValue(chboxes);

            bool exist_value = false;

            for (uint i = 0; i < d.field.commands.size(); i++)
            {
                if (exist_value)
                {
                    break;
                }

                if (value == d.field.commands[i].value)
                {
                    d.field.combo->SetCurrentSelection((int)i);

                    exist_value = true;
                }
            }

            if(!exist_value)
            {
                d.field.combo->SetInvalidChoice();
            }
        }
    }

    for (int num_combo = 0; num_combo < 5; num_combo++)
    {
        if (combo_modes[num_combo])
        {
            ComboRange::UpdateState(modes[num_combo], chboxes, combo_modes[num_combo]);
        }
    }

    OnEventUpdateComboCommandsAndModes();
}


void ComboRange::UpdateState(std::vector<ModeDescripion> &mode_desc, const std::vector<CheckBoxBit *> &chbox, CommandsCombo *combo)
{
    for (uint num_desc = 0; num_desc < mode_desc.size(); num_desc++)
    {
        ModeDescripion &_desc = mode_desc[num_desc];

        std::vector<StateBit> &state_bit_array = _desc.state;

        bool current_state = true;          // Если после следующего цикла это значение будет оставаться правдой,
                                            // это означает, что биты регистра соответствуют данному std::vector<StateBit>

        for (uint num_bit = 0; num_bit < state_bit_array.size(); num_bit++)
        {
            StateBit &state_bit = state_bit_array[num_bit];

            if (chbox[(uint)state_bit.num]->IsChecked() != state_bit.state)
            {
                current_state = false;
                break;
            }
        }

        if (current_state)
        {
            combo->SetCurrentSelection((int)num_desc);
            return;
        }
    }

    combo->SetInvalidChoice();
}


RegDAC::RegDAC(wxWindow *parent, pchar _title, Chip *_chip, const wxString &_functional) : Register(parent, _title, _chip, _functional)
{
    const int d = 10;

    knob = new KnobWidget(painter, 0, 100, 50);
    knob->SetPosition({ painter->GetSize().x - d - 70, d });
    knob->Bind(wxEVT_SLIDER, &RegDAC::OnEventKnob, this);

    slider = new SliderInt(painter, (chip->BitDepth() - 4) * 20, 0, 100, "");
    slider->SetPosition({ painter->BitX(chip->BitDepth() - 5, chip->BitDepth()) + 3, 75 });

    slider->Bind(wxEVT_SLIDER, &RegDAC::OnEventSlider, this);
}


RegAD5531::RegAD5531(wxWindow *_parent, Chip *_chip, const wxString &_functional) :
    RegDAC(_parent, "AD5531", _chip, _functional)
{
}


RegAD5443::RegAD5443(wxWindow *_parent, Chip *_chip, const wxString &_functional) :
    RegDAC(_parent, "AD5443", _chip, _functional)
{
    std::vector<StructDescription> desc0;
    desc0.emplace_back(StructDescription{ 0, NumBitsValue(), "", "", { true } });

    std::vector<StructDescription::CommandStruct> commands;
    commands.emplace_back(StructDescription::CommandStruct{ 0b0000, "No operation" });
    commands.emplace_back(StructDescription::CommandStruct{ 0b0001, "Load and update" });
    commands.emplace_back(StructDescription::CommandStruct{ 0b0010, "Initiate readback" });
    commands.emplace_back(StructDescription::CommandStruct{ 0b1001, "Daisy-chain disable" });
    commands.emplace_back(StructDescription::CommandStruct{ 0b1010, "Clock data to shift register on rising edge" });
    commands.emplace_back(StructDescription::CommandStruct{ 0b1011, "Clear DAC output to zero scale" });
    commands.emplace_back(StructDescription::CommandStruct{ 0b1100, "Clear DAC output to midscale" });
    desc0.emplace_back(StructDescription{ NumBitsValue(), 4, "DAC controls Bits", "DAC controls Bits", {true, commands} });

    SetDescriptionBits(0, desc0);
}

void RegDAC::OnEventKnob(wxCommandEvent &event)
{
    if (event.GetId() == knob->GetId())
    {
        OnEventChangedControl(event.GetInt());
    }

    event.Skip();
}


void RegDAC::OnEventSlider(wxCommandEvent &event)
{
    if (event.GetId() == slider->GetId())
    {
        OnEventChangedControl(event.GetInt());
    }

    event.Skip();
}


void RegDAC::OnEventChangedControl(int value)
{
    int max_value = (1 << NumBitsValue()) - 1;

    uint new_value = (uint)((float)max_value * (float)value / 100.0f + 0.5f);

    SetValueDAC(new_value);
}


int RegDAC::FirstBitValue() const
{
    return 0;
}


int RegDAC::NumBitsValue() const
{
    return GetChip()->BitDepth() - 4;
}


uint Register::GetValue() const
{
    uint result = 0;

    for (uint i = 0; i < chboxes.size(); i++)
    {
        if (chboxes[i]->IsChecked())
        {
            result |= (1 << i);
        }
    }

    return result;
}


void Register::SetValue(uint new_value)
{
    for (uint i = 0; i < chboxes.size(); i++)
    {
        chboxes[i]->SetValue((new_value & (1 << i)) != 0);
    }

    UpdateComboCommandsAndModes();
    UpdateDecFields();
}


void Register::SetValueToBits(uint new_value, int first_bit, int num_bits)
{
    for (int i = first_bit; i < first_bit + num_bits; i++)
    {
        chboxes[(uint)i]->SetValue((new_value & 0b1) != 0);

        new_value >>= 1;
    }

    UpdateComboCommandsAndModes();
    UpdateDecFields();
}


uint Register::GetValueFromBits(int first_bit, int num_bits) const
{
    uint result = 0;

    uint mask = 1;

    for (int i = first_bit; i < first_bit + num_bits; i++)
    {
        if (chboxes[(uint)i]->IsChecked())
        {
            result |= mask;
        }

        mask <<= 1;
    }

    return result;
}


CheckBoxBit::CheckBoxBit(wxWindow *parent, const wxPoint &pos, const wxSize &size) :
    Painter(parent, pos, size)
{
    SetCursor(wxCursor(wxCURSOR_HAND));

    RePaint();

    Bind(wxEVT_LEFT_DOWN, &CheckBoxBit::OnEventLeftClick, this);
}


void CheckBoxBit::SetValue(bool new_value)
{
    value = new_value;

    RePaint();
}


bool CheckBoxBit::IsChecked() const
{
    return value;
}


bool CheckBoxBit::Enable(bool enable)
{
    bool result = Painter::Enable(enable);

    RePaint();

    return result;
}


void CheckBoxBit::RePaint()
{
    BeginPaint(IsEnabled() ? *wxWHITE : *wxLIGHT_GREY);

    wxFont font(7, wxFONTFAMILY_DEFAULT,
        wxFONTSTYLE_NORMAL,
        wxFONTWEIGHT_BOLD);

    font.SetPointSize(10);

    gc->SetFont(font, *wxBLACK);

    gc->DrawText(value ? "1" : "0", 7, 0);

    EndPaint();
}


void CheckBoxBit::OnEventLeftClick(wxMouseEvent &)
{
    value = !value;

    RePaint();

    GF::SendCommandEvent(this, wxEVT_CHECKBOX, value ? 1 : 0);
}


void Register::Pack()
{
    Config::WriteUint(chip->GetNameDevice(), GetValue());
}


void Register::Unpack()
{
    SetValue(Config::ReadUint(chip->GetNameDevice()));
}


void RegDAC::Unpack()
{
    Register::Unpack();
    SetValueToKnobAndSlider();
}


void RegDAC::SetValueToKnobAndSlider()
{
    int max_value = (1 << NumBitsValue()) - 1;

    uint valueDAC = GetValueDAC();

    int new_value = (int)((float)valueDAC * 100.0f / (float)max_value + 0.5f);

    if (new_value != knob->GetValue())
    {
        knob->SetValue(new_value, false);
    }

    if (new_value != slider->GetValue())
    {
        slider->SetValue(new_value, false);
    }
}


void RegDAC::OnEventUpdateComboCommandsAndModes()
{
    SetValueToKnobAndSlider();
}


uint RegDAC::GetValueDAC()
{
    return GetValueFromBits(FirstBitValue(), NumBitsValue());
}


void RegDAC::SetValueDAC(uint value)
{
    if (GetValueDAC() != value)
    {
        SetValueToBits(value, FirstBitValue(), NumBitsValue());
    }
}
