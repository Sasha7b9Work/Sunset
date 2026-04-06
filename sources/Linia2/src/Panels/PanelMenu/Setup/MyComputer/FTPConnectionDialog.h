#pragma once


class FTPConnectionDialog : public wxDialog
{
public:
    FTPConnectionDialog(wxWindow *);
    ~FTPConnectionDialog();

    wxString GetServer() const
    {
        return m_server;
    }
    wxString GetHost() const
    {
        return m_server;
    }  // Алиас для совместимости
    int GetPort() const
    {
        return m_port;
    }
    wxString GetUsername() const
    {
        return m_username;
    }
    wxString GetPassword() const
    {
        return m_password;
    }
    void ClearPassword();

private:
    void CreateControls();
    void OnOK(wxCommandEvent &);
    void OnCancel(wxCommandEvent &);

    wxTextCtrl *m_serverCtrl;
    wxTextCtrl *m_portCtrl;
    wxTextCtrl *m_usernameCtrl;
    wxTextCtrl *m_passwordCtrl;

    wxString m_server;
    int m_port;
    wxString m_username;
    wxString m_password;

    wxDECLARE_EVENT_TABLE();
};
