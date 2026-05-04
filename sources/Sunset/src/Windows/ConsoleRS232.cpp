// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/String.h"
#include "Communicator/ComPort/ComPort.h"
#include "Settings/Settings.h"
#include "Windows/ConsoleRS232.h"
#pragma warning(push, 0)
#include <wx/textctrl.h>
#pragma warning(pop)


ConsoleRS232 *ConsoleRS232::self = nullptr;


enum
{
    ID_LINE = wxID_HIGHEST + 1
};


ConsoleRS232::ConsoleRS232(wxFrame *parent) : wxFrame(parent, wxID_ANY, L("ИППП4"))
{
    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);

    line = new wxTextCtrl(this, ID_LINE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    line->SetFocus();

    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, L("Courier New"));
    line->SetFont(font);
    text->SetFont(font);

    Bind(wxEVT_SIZE, &ConsoleRS232::OnSize, this);
    line->Bind(wxEVT_TEXT_ENTER, &ConsoleRS232::OnEnterKey, this, ID_LINE);
    line->Bind(wxEVT_KEY_DOWN, &ConsoleRS232::OnTextControlKeyDown, this, ID_LINE);
    Bind(wxEVT_CLOSE_WINDOW, &ConsoleRS232::OnClose, this);
}

ConsoleRS232::~ConsoleRS232()
{
    self = nullptr;
}


void ConsoleRS232::OnSize(wxSizeEvent &)
{
    wxPoint clientOrigin = GetClientAreaOrigin();

    wxSize clientSize = GetClientSize();

    int heightLine = line->GetSize().y;

    wxSize sizeText = clientSize;
    sizeText.y -= heightLine;

    text->SetSize(sizeText);

    line->SetPosition({ clientOrigin.x, clientSize.y - heightLine });
    line->SetSize({ text->GetSize().x, heightLine });
}


void ConsoleRS232::Create()
{
    if (!self)
    {
        self = new ConsoleRS232(nullptr);

        self->SetPosition(SET::GUI::pos_console->Get());

        self->SetClientSize(SET::GUI::size_console->Get().x, SET::GUI::size_console->Get().y);

        self->Maximize(SET::GUI::maximized_console->Get());

        self->Show(false);
    }
}


void ConsoleRS232::OnEnterKey(wxCommandEvent &)
{
    String command(line->GetLineText(0).c_str());

    history.Add(command.c_str());

    if (command[0] == '#')
    {
    }
    else
    {
        String first_word = command.GetWord(1, " \0");
        first_word.ToUpper();

        if (first_word == "HELP")
        {
            AddLine("Команды:");
            AddLine("\tCtrl-K - открыть/закрыть консоль");
            AddLine("\tclear - очистить консоль");
            //AddLine("\tpages - показать дополнительные вкладки");
        }
        else
        {
            if (first_word == "CLEAR")
            {
                text->Clear();
            }
        }
    }

    line->Clear();
}


void ConsoleRS232::OnTextControlKeyDown(wxKeyEvent &event)
{
    if (event.GetKeyCode() == WXK_UP)
    {
        wxString txt = history.Prev();

        if (txt.size())
        {
            line->Clear();
            line->WriteText(txt);
        }
    }
    else if (event.GetKeyCode() == WXK_DOWN)
    {
        wxString txt = history.Next();

        if (txt.size())
        {
            line->Clear();
            line->WriteText(txt);
        }
    }
    else
    {
        event.Skip();
    }
}


void ConsoleRS232::AddLine(const wxString &str)
{
    mutex.lock();

    text->WriteText(str);
    text->WriteText(wxT("\n"));

    mutex.unlock();
}


void ConsoleRS232::SwitchVisibility()
{
    self->Show(!self->IsShown());
}


void ConsoleRS232::OnClose(wxCloseEvent &)
{
    self->Show(false);
}


void ConsoleRS232::History::Add(const wxString &txt)
{
    if ((history.size() == 0) || 
        (history[history.size() - 1].compare(txt) != 0))
    {
        history.push_back(txt);
        position = history.size() - 1;
    }
}


wxString ConsoleRS232::History::Next()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position++;
    if (position == history.size())
    {
        position = 0;
    }

    return result;
}


wxString ConsoleRS232::History::Prev()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position = (position == 0) ? (history.size() - 1) : (position - 1);

    return result;
}
