#pragma once
#include "Controls/Dialog.h"
#include "MyComputer/FilePanel.h"
#include "MyComputer/FilePanelEvents.h"


class WindowMyComputer : public Dialog
{
public:
    static const int WIDTH = 1024;
    static const int HEIGHT = 768;

    WindowMyComputer();
    virtual ~WindowMyComputer();

    void SetActivePanel(FilePanel *);
    void UpdateStatus(const wxString &message);

    // Обработчики событий
    void OnTabKey(wxKeyEvent &event);
    void OnClose(wxCommandEvent &event);
    void OnStatusUpdate(wxCommandEvent &event);
    void OnPanelActivate(wxCommandEvent &event);

private:

    void CreateControls();
    void SetupEventHandlers();

    wxSplitterWindow *m_splitter;
    FilePanel *m_leftPanel;
    FilePanel *m_rightPanel;
    wxStaticText *m_statusText;
    FilePanel *m_activePanel;
};
