// 2025/10/17 08:25:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class AutoRebootDialog : public wxDialog
{
public:
    AutoRebootDialog(wxWindow *parent, const wxString &message, int countdownSeconds, void (*_func_on_finish)()) :
        wxDialog(parent, wxID_ANY, "Ошибка", wxDefaultPosition, wxSize(350, 170), wxCAPTION | wxCLOSE_BOX | wxRESIZE_BORDER),
        m_countdown(countdownSeconds),
        m_initialCountdown(countdownSeconds),
        func_on_finish(_func_on_finish)
    {
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        // Текст сообщения
        m_messageText = new wxStaticText(this, wxID_ANY, message);
        mainSizer->Add(m_messageText, 0, wxALL | wxALIGN_CENTER, 10);

        // Текст отсчета
        m_countdownText = new wxStaticText(this, wxID_ANY,
            wxString::Format(wxT("Перезагрузка через %d секунд"), m_countdown));
        mainSizer->Add(m_countdownText, 0, wxALL | wxALIGN_CENTER, 10);

        // Кнопки
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        m_okButton = new wxButton(this, wxID_OK, "Перезагрузить");
        m_cancelButton = new wxButton(this, wxID_CANCEL, "Отмена");

        buttonSizer->Add(m_okButton, 0, wxALL, 5);
        buttonSizer->Add(m_cancelButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 10, wxALIGN_CENTER | wxBOTTOM, 20);

        SetSizer(mainSizer);
        wxWindowBase::Centre();

        // Настройка таймера
        m_timer = new wxTimer(this, wxID_ANY);
        m_timer->Start(1000); // Таймер срабатывает каждую секунду

        // Привязка событий кнопок
        m_okButton->Bind(wxEVT_BUTTON, &AutoRebootDialog::OnOk, this);
        m_cancelButton->Bind(wxEVT_BUTTON, &AutoRebootDialog::OnCancel, this);

        Bind(wxEVT_TIMER, &AutoRebootDialog::OnTimer, this);
    }

    ~AutoRebootDialog()
    {
        if (m_timer && m_timer->IsRunning())
        {
            m_timer->Stop();
        }
    }

private:
    void OnTimer(wxTimerEvent &)
    {
        m_countdown--;

        if (m_countdown <= 0)
        {
            m_timer->Stop();
            // Выполняем действие по достижении нуля
            OnCountdownFinished();
        }
        else
        {
            // Обновляем текст отсчета
            m_countdownText->SetLabel(
                wxString::Format(wxT("Перезагрузка через %d секунд"), m_countdown));
            Layout();
        }
    }

    void OnCountdownFinished()
    {
        // Закрываем диалог с кодом OK
        EndModal(wxID_OK);

        func_on_finish();
    }

    void OnOk(wxCommandEvent &)
    {
        m_timer->Stop();
        EndModal(wxID_OK);

        func_on_finish();
    }

    void OnCancel(wxCommandEvent &)
    {
        m_timer->Stop();
        EndModal(wxID_CANCEL);
    }

private:
    wxStaticText *m_messageText;
    wxStaticText *m_countdownText;
    wxButton *m_okButton;
    wxButton *m_cancelButton;
    wxTimer *m_timer;
    int m_countdown;
    int m_initialCountdown;
    void (*func_on_finish)() = nullptr;
};
