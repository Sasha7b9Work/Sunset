// 2025/10/17 08:25:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Button.h"
#pragma warning(push, 0)
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#pragma warning(pop)


class Button;


class AutoRebootDialog : public wxDialog
{
public:
    AutoRebootDialog(wxWindow *parent, const wxString &message, int countdownSeconds, void (*_func_on_finish)()) :
        wxDialog(parent, wxID_ANY, "Ошибка", wxDefaultPosition, wxSize(350, 170), wxCAPTION | wxCLOSE_BOX | wxRESIZE_BORDER),
        countdown(countdownSeconds),
        initialCountdown(countdownSeconds),
        func_on_finish(_func_on_finish)
    {
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        // Текст сообщения
        messageText = new wxStaticText(this, wxID_ANY, message);
        mainSizer->Add(messageText, 0, wxALL | wxALIGN_CENTER, 10);

        // Текст отсчета
        countdownText = new wxStaticText(this, wxID_ANY, wxString::Format(L("Перезагрузка через %d секунд"), countdown));
        mainSizer->Add(countdownText, 0, wxALL | wxALIGN_CENTER, 10);

        // Кнопки
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        okButton = new Button(this, L("Перезагрузить"));
        cancelButton = new Button(this, L("Отмена"));

        buttonSizer->Add(okButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 10, wxALIGN_CENTER | wxBOTTOM, 20);

        SetSizer(mainSizer);
        wxWindowBase::Centre();

        // Настройка таймера
        timer = new wxTimer(this, wxID_ANY);
        timer->Start(1000); // Таймер срабатывает каждую секунду

        // Привязка событий кнопок
        okButton->Bind(wxEVT_BUTTON, &AutoRebootDialog::OnOk, this);
        cancelButton->Bind(wxEVT_BUTTON, &AutoRebootDialog::OnCancel, this);

        Bind(wxEVT_TIMER, &AutoRebootDialog::OnTimer, this);
    }

    ~AutoRebootDialog()
    {
        if (timer && timer->IsRunning())
        {
            timer->Stop();
        }
    }

private:
    void OnTimer(wxTimerEvent &)
    {
        countdown--;

        if (countdown <= 0)
        {
            timer->Stop();
            // Выполняем действие по достижении нуля
            OnCountdownFinished();
        }
        else
        {
            // Обновляем текст отсчета
            countdownText->SetLabel(wxString::Format(L("Перезагрузка через %d секунд"), countdown));
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
        timer->Stop();
        EndModal(wxID_OK);

        func_on_finish();
    }

    void OnCancel(wxCommandEvent &)
    {
        timer->Stop();
        EndModal(wxID_CANCEL);
    }

private:
    wxStaticText *messageText;
    wxStaticText *countdownText;
    Button       *okButton;
    Button       *cancelButton;
    wxTimer      *timer;
    int           countdown;
    int           initialCountdown;
    void (*func_on_finish)() = nullptr;
};
