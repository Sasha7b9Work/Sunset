#include "defines.h"
#include "FTPConnectionDialog.h"


wxBEGIN_EVENT_TABLE(FTPConnectionDialog, wxDialog)
EVT_BUTTON(wxID_OK, FTPConnectionDialog::OnOK)
EVT_BUTTON(wxID_CANCEL, FTPConnectionDialog::OnCancel)
wxEND_EVENT_TABLE()

// Безопасная очистка строки — перезаписывает содержимое нулями
static void SecureClearString(wxString &str)
{
    if (!str.IsEmpty())
    {
        // Перезаписываем каждый символ
        for (size_t i = 0; i < str.length(); ++i)
        {
            str[i] = '\0';
        }
        str.Clear();
    }
}

FTPConnectionDialog::FTPConnectionDialog(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, "Подключение к FTP серверу",
        wxDefaultPosition, wxSize(400, 300)),
    m_serverCtrl(nullptr),
    m_portCtrl(nullptr),
    m_usernameCtrl(nullptr),
    m_passwordCtrl(nullptr),
    m_server(""),
    m_port(22),  // SFTP использует порт 22 по умолчанию
    m_username(""),
    m_password("")
{
    CreateControls();
    Center();
}

FTPConnectionDialog::~FTPConnectionDialog()
{
    ClearPassword();
}

void FTPConnectionDialog::ClearPassword()
{
    SecureClearString(m_password);
}

void FTPConnectionDialog::CreateControls()
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Сервер
    wxBoxSizer *serverSizer = new wxBoxSizer(wxHORIZONTAL);
    serverSizer->Add(new wxStaticText(this, wxID_ANY, "Сервер:",
        wxDefaultPosition, wxSize(100, -1)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    m_serverCtrl = new wxTextCtrl(this, wxID_ANY, "");
    serverSizer->Add(m_serverCtrl, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(serverSizer, 0, wxEXPAND | wxALL, 5);

    // Порт (по умолчанию 22 для FTP)
    wxBoxSizer *portSizer = new wxBoxSizer(wxHORIZONTAL);
    portSizer->Add(new wxStaticText(this, wxID_ANY, "Порт:",
        wxDefaultPosition, wxSize(100, -1)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    m_portCtrl = new wxTextCtrl(this, wxID_ANY, "22");
    portSizer->Add(m_portCtrl, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(portSizer, 0, wxEXPAND | wxALL, 5);

    // Имя пользователя
    wxBoxSizer *userSizer = new wxBoxSizer(wxHORIZONTAL);
    userSizer->Add(new wxStaticText(this, wxID_ANY, "Пользователь:",
        wxDefaultPosition, wxSize(100, -1)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    m_usernameCtrl = new wxTextCtrl(this, wxID_ANY, "");
    userSizer->Add(m_usernameCtrl, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(userSizer, 0, wxEXPAND | wxALL, 5);

    // Пароль
    wxBoxSizer *passSizer = new wxBoxSizer(wxHORIZONTAL);
    passSizer->Add(new wxStaticText(this, wxID_ANY, "Пароль:", wxDefaultPosition, wxSize(100, -1)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    m_passwordCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    passSizer->Add(m_passwordCtrl, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(passSizer, 0, wxEXPAND | wxALL, 5);

    // Разделитель
    mainSizer->AddSpacer(10);

    // Кнопки
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->AddStretchSpacer(1);
    buttonSizer->Add(new wxButton(this, wxID_OK, "Подключить"), 0, wxALL, 5);
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Отмена"), 0, wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 10);

    SetSizer(mainSizer);
    mainSizer->Fit(this);
    mainSizer->SetSizeHints(this);
}

void FTPConnectionDialog::OnOK(wxCommandEvent & /*event*/)
{
    m_server = m_serverCtrl->GetValue();
    wxString portStr = m_portCtrl->GetValue();
    long portLong;
    if (portStr.ToLong(&portLong) && portLong >= 1 && portLong <= 65535)
    {
        m_port = static_cast<int>(portLong);
    }
    else
    {
        wxMessageBox("Порт должен быть числом от 1 до 65535", "Ошибка", wxOK | wxICON_ERROR);
        return;
    }
    m_username = m_usernameCtrl->GetValue();
    m_password = m_passwordCtrl->GetValue();

    if (m_server.IsEmpty())
    {
        wxMessageBox("Введите адрес сервера", "Ошибка", wxOK | wxICON_ERROR);
        return;
    }

    if (m_username.IsEmpty())
    {
        wxMessageBox("Введите имя пользователя", "Ошибка", wxOK | wxICON_ERROR);
        return;
    }

    EndModal(wxID_OK);
}

void FTPConnectionDialog::OnCancel(wxCommandEvent & /*event*/)
{
    EndModal(wxID_CANCEL);
}
