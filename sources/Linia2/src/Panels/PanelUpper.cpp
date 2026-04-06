// 2025/6/1 17:14:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelUpper.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"
#include "Communicator/ComPort/ComPort.h"
#include "Settings/Settings.h"


PanelUpper *PanelUpper::self = nullptr;


PanelUpper::PanelUpper(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1, 0, MainWindow::WIDTH_DRAW - MainWindow::WIDTH1, MainWindow::HEIGHT_HI)
{
    self = this;

    if (SET::emulate_mode->Get())
    {
        wxFont bigFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        wxClientDC dc(this);
        dc.SetFont(bigFont);
        wxSize textSize = dc.GetTextExtent(wxT("Э М У Л Я Т О Р"));
        wxStaticText *text = new wxStaticText(this, wxID_ANY, "Э М У Л Я Т О Р", { 200, 0 }, wxSize(textSize.GetWidth(), textSize.GetHeight()));
        text->SetFont(bigFont);
        text->SetForegroundColour(wxColour(127, 127, 127));
        text->SetSize(text->GetBestSize());
        text->Refresh();
    }

    new wxStaticText(this, wxID_ANY, wxString::Format("ver. %d : %s", VERSION_BUILD, DATE_BUILD), { 450, 56 });

    new wxStaticText(this, wxID_ANY, wxString::Format("IP : %s", GF::GetSelfIP().c_str().AsChar()), {10, 56});

#ifdef WIN32

    new wxStaticText(this, wxID_ANY, "COM-порт", { 150, 50 });

    comboPorts = new wxComboBox(this, wxID_ANY, "", { 220, 45 });

    color_square = new ColorSquare(this, wxID_ANY);
    color_square->SetPosition({ 340, 45 });

    std::vector<bool> ports;

    ComPort::GetComports(ports);

    for (size_t i = 0; i < ports.size(); i++)
    {
        if (ports[i])
        {
            comboPorts->Append(wxString::Format("COM%d", (int)i + 1));
        }
    }

    SetTunedPort();

    Bind(wxEVT_COMBOBOX, &PanelUpper::OnEventComboBox, this);

#endif
}


#ifdef WIN32

void PanelUpper::OnEventComboBox(wxCommandEvent &event)
{
    if (event.GetId() == comboPorts->GetId())
    {
        ComPort::Disconnect();

        ComPort::Connect(GetNumPort());
    }
}


void PanelUpper::SetTunedPort()
{
    wxString pattern_end = wxString::Format("%d", SET::GUI::serial_port_num->Get());

    for (uint i = 0; i < comboPorts->GetCount(); i++)
    {
        wxString item = comboPorts->GetString(i);

        if (item.EndsWith(pattern_end))
        {
            comboPorts->SetSelection((int)i);
            return;
        }
    }

    if (comboPorts->GetCount())
    {
        comboPorts->SetSelection(0);
    }
}


int PanelUpper::GetNumPort() const
{
    if (comboPorts->GetCount() == 0)
    {
        return false;
    }

    uint selection = (uint)comboPorts->GetSelection();

    wxString name_port = comboPorts->GetString(selection);

    int i = (int)name_port.Length() - 1;

    while (i >= 0 && wxIsdigit(name_port[i]))
    {
        i--;
    }

    int num_port = -1;

    (name_port.Mid((size_t)(i + 1))).ToInt(&num_port);

    return num_port - 1;
}

#endif
