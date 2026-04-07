// 2025/09/03 09:54:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageOrangePi.h"
#include "Utils/SystemDepend.h"
#include "Communicator/UART/UART.h"
#include "Utils/Timer.h"
#include "MainWindow.h"
#ifdef WIN32
#else
    #include <sched.h>
#endif


PageOrangePi *ThePageOrangePi = nullptr;

bool PageOrangePi::thread_is_running = false;
bool PageOrangePi::thread_autoUART_is_running = false;


PageOrangePi::PageOrangePi(wxNotebook *parent) :
    wxPanel(parent)
{
    SetBackgroundColour(parent->GetBackgroundColour());

    ThePageOrangePi = this;

    wxPanel::SetName("Orange Pi 5");

    wxStaticBox *boxUART = new wxStaticBox(this, wxID_ANY, "UART",
        (boxGPIO != nullptr) ? wxPoint{ boxGPIO->GetPosition().x + boxGPIO->GetSize().x + 10, 10 } : wxPoint{10, 10},
        { 200, 270 });

    {
        btnReinitUart = new wxButton(boxUART, wxID_ANY, "Reinit", { 10, SD::Y_SB(230) }, { 100, 20 });

        new wxStaticText(boxUART, wxID_ANY, "TX : 8", { 10, SD::Y_SB(20) });
        new wxStaticText(boxUART, wxID_ANY, "RX : 10", { 70, SD::Y_SB(20) });

        txtSendUART = new wxTextCtrl(boxUART, wxID_ANY, "", { 10, SD::Y_SB(50) }, { 170, 20 });
        btnSendUART = new wxButton(boxUART, wxID_ANY, "Send", { 10, SD::Y_SB(75) }, { 100, 20 });
        btnAutoUART = new wxToggleButton(boxUART, wxID_ANY, "AutoSend", { 10, SD::Y_SB(100) }, { 100, 20 });

        new wxStaticText(boxUART, wxID_ANY, "Принято:", { 10, SD::Y_SB(130) });
        txtRecvUART = new wxTextCtrl(boxUART, wxID_ANY, "", { 10, SD::Y_SB(150) }, { 150, 60 }, wxTE_READONLY | wxTE_MULTILINE);
    }

    wxStaticBox *boxSPI = new wxStaticBox(this, wxID_ANY, "SPI", { boxUART->GetPosition().x + boxUART->GetSize().x + 10, 10 }, { 200, 270 });

    {
        new wxStaticText(boxSPI, wxID_ANY, "MOSI : 19", { 10, SD::Y_SB(20) });
        new wxStaticText(boxSPI, wxID_ANY, "CLK : 23", { 10, SD::Y_SB(45) });

        new wxTextCtrl(boxSPI, wxID_ANY, "", { 10, SD::Y_SB(70) }, { 100, 20 });
        btnSendSPI = new wxButton(boxSPI, wxID_ANY, "Send", { 120, SD::Y_SB(70) }, { 50, 20 });
    }

//    CreateBoxEncoder({ boxSPI->GetPosition().x + boxSPI->GetSize().x + 10, 10 });

    wxSize size_button{ 75, BUTTON_HEIGHT };
    btnReturn = new wxButton(this, wxID_ANY, "Закрыть", { MainWindow::WIDTH - size_button.x - 15, 0 }, size_button);

    Bind(wxEVT_BUTTON, &PageOrangePi::OnEventButton, this);
    Bind(wxEVT_TOGGLEBUTTON, &PageOrangePi::OnEventToggleButton, this);
}


void PageOrangePi::CreateBoxGPIO()
{
    boxGPIO = new wxStaticBox(this, wxID_ANY, "GPIO", { 10, 10 }, { 200, 270 });

    {
        int x = 10;
        int y = 20;
        int dy = 31;


        static PinIn *pins_in[] =
        {
            &pinSTART,
            &pinSTOP,
            &pinDAT_F0,
            &pinSPI_MOSI,
            &pinDAT_F2,
            &pinFIFO_FULL,
            nullptr
        };

        pinSTART.SetChangeCallback(CallbackOnStart);

        //        pinFIFO_FULL.SetChangeCallback(CallbackOnFIFO_FULL);

        int index = 0;

        PinIn *in = pins_in[index];

        while (in)
        {
            wxPanel *panel = CreatePanelPinIn(boxGPIO, in);

            panel->SetPosition({ x, SD::Y_SB(y) });

            y += dy;

            in = pins_in[++index];
        }

        static PinOut *pins_out[] =
        {
            &pinREQ_RD,
            nullptr
        };

        index = 0;

        PinOut *out = pins_out[index];

        while (out)
        {
            wxPanel *panel = CreatePanelPinOut(boxGPIO, out);

            panel->SetPosition({ x, SD::Y_SB(y) });

            y += dy;

            out = pins_out[++index];
        }
    }
}


wxStaticBox *PageOrangePi::CreateBoxEncoder(const wxPoint &position)
{
    wxStaticBox *boxEncoder = new wxStaticBox(this, wxID_ANY, "Encoder", position, { 200, 270 });

    {
        new wxStaticText(boxEncoder, wxID_ANY, "KA : 11", { 10, SD::Y_SB(20) });
        _txtKA = new wxTextCtrl(boxEncoder, wxID_ANY, "0", { 60, SD::Y_SB(20) }, { 60, 20 }, wxTE_READONLY);

        new wxStaticText(boxEncoder, wxID_ANY, "KB : 13", { 10, SD::Y_SB(45) });
        _txtKB = new wxTextCtrl(boxEncoder, wxID_ANY, "0", { 60, SD::Y_SB(45) }, { 60, 20 }, wxTE_READONLY);
    }

    return boxEncoder;
}


wxString PageOrangePi::NamePin(Pin::E pin) const
{
    static const pchar names[Pin::Count] =
    {
        "START",
        "STOP",
        "DAT_F0",
        "SPI_MOSI",
        "DAT_F2",
        "SPI_CS",
        "FIFO_FULL",
        "REQ_RD"
    };

    return names[pin];
}


int PageOrangePi::NumPin(Pin::E pin) const
{
    static const int num[Pin::Count] =
    {
        15,
        21,
        16,
        18,
        22,
        24,
        36,
        32
    };

    return num[pin];
}


wxPanel *PageOrangePi::CreatePanelPinOut(wxWindow *parent, PinOut *pin)
{
    wxPanel *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, { 180, 23 });

    StructOutGPIO strGPIO(pin);

    strGPIO.button = new wxButton(panel, wxID_ANY, NamePin(pin->type()), { 0, 0 }, { 70, 22 });

    strGPIO.button->Bind(wxEVT_BUTTON, &PageOrangePi::OnEventButton, this);

    strGPIO._txtStatePull = new wxTextCtrl(panel, wxID_ANY, "", { 100, 0 }, { 20, 22 }, wxTE_READONLY);

    strGPIO._txtStateInt = new wxTextCtrl(panel, wxID_ANY, "", { 130, 0 }, { 20, 22 }, wxTE_READONLY);

    new wxStaticText(panel, wxID_ANY, wxString::Format("%d", NumPin(pin->type())), { 160, 2 }, { 40, 22 });

    gpio_out.push_back(strGPIO);

    return panel;
}


wxPanel *PageOrangePi::CreatePanelPinIn(wxWindow *parent, PinIn *pin)
{
    wxPanel *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, { 180, 23 });

    StructInGPIO strGPIO(pin);

    new wxStaticText(panel, wxID_ANY, NamePin(pin->type()), { 0, 2 }, { 80, 22 });

    strGPIO._txtStatePull = new wxTextCtrl(panel, wxID_ANY, "", { 100, 0 }, { 20, 22 }, wxTE_READONLY);

    strGPIO._txtStateInt = new wxTextCtrl(panel, wxID_ANY, "", { 130, 0 }, { 20, 22 }, wxTE_READONLY);

    new wxStaticText(panel, wxID_ANY, wxString::Format("%d", NumPin(pin->type())), { 160, 2 }, { 40, 22 });

    gpio_in.push_back(strGPIO);

    return panel;
}


void PageOrangePi::OnEventButton(wxCommandEvent &event)
{
    event.Skip();

    int id = event.GetId();

    if (id == btnSendSPI->GetId())
    {

    }
    else if (id == btnSendUART->GetId())
    {
        wxString message = wxString::Format("%s", ThePageOrangePi->txtSendUART->GetValue().c_str().AsChar());

        UART::SendBuffer(message.GetData().AsChar(), (int)(std::strlen(message.GetData().AsChar()) + 1));
    }
    else if (id == btnReturn->GetId())
    {
        TheMainWindow->SetMode(ModeMainWindow::Standard);
    }
    else if (id == btnReinitUart->GetId())
    {
        UART::ReInit();
    }
    else
    {
        for (auto &pin : gpio_out)
        {
            if (id == pin.button->GetId())
            {
                return;
            }
        }
    }
}


void PageOrangePi::OnEventToggleButton(wxCommandEvent &event)
{
    event.Skip();

    int id = event.GetId();

    if (id == btnAutoUART->GetId())
    {
        bool enable = event.GetInt() == 0;

        btnSendUART->Enable(enable);
        txtSendUART->Enable(enable);

        if (event.GetInt())
        {
            thread_autoUART_is_running = true;

            thread_UART = new std::thread(ThreadFuncAutoUART);

            thread_UART->detach();
        }
        else
        {
            thread_autoUART_is_running = false;

            while (thread_UART->joinable())
            {
            }

            SAFE_DELETE(thread_UART);
        }
    }
}


void PageOrangePi::ThreadFunc()
{
    while (thread_is_running)
    {
        for (auto &str : ThePageOrangePi->gpio_out)
        {
            str.value_pull = str.pin->Get() ? 1 : 0;
        }

        for (auto &str : ThePageOrangePi->gpio_in)
        {
            str.value_int = str.pin->Get() ? 1 : 0;
        }

        if (ThePageOrangePi->_txtKA)
        {
            ThreadFuncEncoder();
        }
    }
}


void PageOrangePi::FuncOnRecvUART(char byte)
{
    ThePageOrangePi->mutex_str_UART.lock();
    ThePageOrangePi->bytesUART.push_back(byte);
    ThePageOrangePi->mutex_str_UART.unlock();
}


void PageOrangePi::FuncUpdateUART()
{
    std::vector<char> b;
    ThePageOrangePi->mutex_str_UART.lock();
    if (ThePageOrangePi->bytesUART.size())
    {
        b = ThePageOrangePi->bytesUART;
        ThePageOrangePi->bytesUART.clear();
    }
    ThePageOrangePi->mutex_str_UART.unlock();

    if (b.size())
    {
        for (uint i = 0; i < b.size(); i++)
        {
            static wxString text;

            char symbol = b[i];

            if (symbol != 0x00)
            {
                text.Append(symbol);
            }
            else
            {
                ThePageOrangePi->txtRecvUART->SetValue(text);
                text.Clear();
            }
        }
    }
}


void PageOrangePi::PeriodicTask()
{
//    for (auto &str : ThePageOrangePi->gpio_out)
//    {
//        str._txtStatePull->SetValue(wxString::Format("%d", str.value_pull));
//    }
//
//    for (auto &str : ThePageOrangePi->gpio_in)
//    {
//        str._txtStatePull->SetValue(wxString::Format("%d", str.value_int));
//    }
//
//    if (ThePageOrangePi->_txtKA)
//    {
//        ThePageOrangePi->_txtKA->SetValue(wxString::Format("%d", valueKA));
//        ThePageOrangePi->_txtKB->SetValue(wxString::Format("%d", valueKB));
//    }

//    FuncUpdateUART();
}


void PageOrangePi::ThreadFuncEncoder()
{
    static bool prevKA = false;
    static bool prevKB = false;

    static bool first = true;

    for (int i = 0; i < 50; i++)
    {
        if (first)
        {
            first = false;
            prevKA = pinKA.Get();
            prevKB = pinKB.Get();
        }
        else
        {
            bool valKA = pinKA.Get();
            bool valKB = pinKB.Get();

            if (valKA != prevKA)
            {
                prevKA = valKA;

                ThePageOrangePi->valueKA++;
            }

            if (valKB != prevKB)
            {
                prevKB = valKB;

                ThePageOrangePi->valueKB++;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


uint8 PageOrangePi::CalculateCRC(uint16 values[4])
{
    BitSet16 bs0{ values[0] };

    uint8 xor0 = (uint8)(bs0.u8[0] ^ bs0.u8[1]);

    BitSet16 bs1{ values[1] };

    uint8 xor1 = (uint8)(bs1.u8[0] ^ bs1.u8[1]);

    BitSet16 bs2{ values[2] };

    uint8 xor2 = (uint8)(bs2.u8[0] ^ bs2.u8[1]);

    BitSet16 bs3{ values[3] };

    uint8 xor3 = (uint8)(bs3.u8[0] ^ bs3.u8[1]);

    uint8 xor01 = (uint8)(xor0 ^ xor1);
    uint8 xor23 = (uint8)(xor2 ^ xor3);

    uint8 crc = (uint8)(xor01 ^ xor23);

    return crc;
}


void PageOrangePi::ThreadFuncAutoUART()
{
    while (thread_autoUART_is_running)
    {
        wxString message = wxString::Format("%s", ThePageOrangePi->txtSendUART->GetValue().c_str().AsChar());

        UART::SendBuffer(message.GetData().AsChar(), (int)(std::strlen(message.GetData().AsChar()) + 1));

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void PageOrangePi::OnChangeStatePin(PinIn *pin, bool state)
{
    for (auto &str : ThePageOrangePi->gpio_in)
    {
        if (str.pin == pin)
        {
            str._txtStateInt->SetValue(state ? "1" : "0");
        }
    }
}


void PageOrangePi::OnChangeStatePin(PinOut *pin, bool state)
{
    for (auto &str : ThePageOrangePi->gpio_out)
    {
        if (str.pin == pin)
        {
            str._txtStateInt->SetValue(state ? "1" : "0");
        }
    }
}


#ifdef WIN32
#else
void set_thread_priority_linux(std::thread &thread, int policy, int priority)
{
    pthread_t handle = thread.native_handle();

    sched_param sch_params;
    sch_params.sched_priority = priority;

    if (pthread_setschedparam(handle, policy, &sch_params) != 0)
    {
        LOG_ERROR("Priority gpio read thread %d is FAIL", priority);
    }
    else
    {
        LOG_WRITE("Priority gpio read thread %d is OK", priority);
    }
}
#endif


void PageOrangePi::Init()
{
//    if (!_thread)
//    {
//        thread_is_running = true;
//        _thread = new std::thread(ThreadFunc);
//#ifdef WIN32
//#else
//        set_thread_priority_linux(*_thread, SCHED_FIFO, 99);
//#endif
//        _thread->detach();
//    }

    is_init = true;
}


bool PageOrangePi::IsInit() const
{
    return is_init;
}


void PageOrangePi::DeInit()
{
    is_init = false;

    thread_is_running = false;
 
    if (_thread)
    {
        while (_thread->joinable())
        {
        }

        SAFE_DELETE(_thread);
    }
}


void PageOrangePi::CallbackOnStart(bool state)
{
    ThePageOrangePi->OnChangeStatePin(&pinSTART, state);
}

void PageOrangePi::CallbackOnStop(bool state)
{
    ThePageOrangePi->OnChangeStatePin(&pinSTOP, state);
}

void PageOrangePi::CallbackOnDAT_F0(bool state)
{
    ThePageOrangePi->OnChangeStatePin(&pinDAT_F0, state);
}

void PageOrangePi::CallbackOnDAT_F1(bool state)
{
    ThePageOrangePi->OnChangeStatePin(&pinSPI_MOSI, state);
}

void PageOrangePi::CallbackOnDAT_F2(bool state)
{
    ThePageOrangePi->OnChangeStatePin(&pinDAT_F2, state);
}

void PageOrangePi::CallbackOnDAT_F3(bool /*state*/)
{
//    ThePageOrangePi->OnChangeStatePin(&pinDAT_F3, state);
}

void PageOrangePi::CallbackOnFIFO_FULL(bool state)
{
    ThePageOrangePi->OnChangeStatePin(&pinFIFO_FULL, state);
}

void PageOrangePi::CallbackonREQ_RD(bool state)
{
    ThePageOrangePi->OnChangeStatePin(&pinREQ_RD, state);
}


void PageOrangePi::Pack()
{

}


void PageOrangePi::Unpack()
{

}
