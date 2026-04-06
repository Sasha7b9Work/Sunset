#include "defines.h"
#include "WindowMyComputer.h"
#include "MyComputer/FilePanel.h"
#include "MyComputer/FilePanelEvents.h"


WindowMyComputer::WindowMyComputer() :
    //    Dialog(wxT("Мой компьютер"), wxDefaultPosition, { WIDTH, HEIGHT })
    Dialog(_("Мой компьютер"), wxDefaultPosition, { WIDTH, HEIGHT }),
    m_splitter(nullptr),
    m_leftPanel(nullptr),
    m_rightPanel(nullptr),
    m_statusText(nullptr),
    m_activePanel(nullptr)
{
    SetMinSize(wxSize(600, 600));
    CreateControls();
    SetupEventHandlers();

    SetActivePanel(m_leftPanel);
    Center();
}

WindowMyComputer::~WindowMyComputer()
{}

void WindowMyComputer::CreateControls()
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    main_panel->SetSizer(mainSizer);

    // Резервируем место для шапки (title bar рисуется в верхних titleHeight пикселях main_panel)
    mainSizer->AddSpacer(TopY());

    m_splitter = new wxSplitterWindow(main_panel, wxID_ANY,
        wxDefaultPosition, wxDefaultSize,
        wxSP_3D | wxSP_LIVE_UPDATE);

    m_leftPanel = new FilePanel(m_splitter, FilePanel::MODE_NONE);
    m_leftPanel->SetId(1001);
    m_rightPanel = new FilePanel(m_splitter, FilePanel::MODE_BUTTONS);
    m_rightPanel->SetId(1002);

    m_splitter->SplitVertically(m_leftPanel, m_rightPanel);
    m_splitter->SetSashGravity(0.5);
    m_splitter->SetMinimumPaneSize(300);

    mainSizer->Add(m_splitter, 1, wxEXPAND | wxALL, 5);

    m_statusText = new wxStaticText(main_panel, wxID_ANY, "Готово");
    mainSizer->Add(m_statusText, 0, wxALL | wxEXPAND, 5);

    // Устанавливаем позицию разделителя после создания всех элементов
    Layout();
    m_splitter->SetSashPosition(m_splitter->GetSize().GetWidth() / 2);
}

void WindowMyComputer::SetupEventHandlers()
{
    Bind(wxEVT_BUTTON, &WindowMyComputer::OnClose, this, wxID_CANCEL);
    Bind(wxEVT_CHAR_HOOK, &WindowMyComputer::OnTabKey, this);
    Bind(wxEVT_FILEPANEL_STATUS, &WindowMyComputer::OnStatusUpdate, this);
    Bind(wxEVT_FILEPANEL_ACTIVATED, &WindowMyComputer::OnPanelActivate, this);
}

void WindowMyComputer::SetActivePanel(FilePanel *panel)
{
    if (m_activePanel == panel) return;
    if (m_activePanel) m_activePanel->SetActive(false);
    m_activePanel = panel;
    if (m_activePanel) m_activePanel->SetActive(true);
}

void WindowMyComputer::UpdateStatus(const wxString &message)
{
    if (m_statusText)
    {
        m_statusText->SetLabel(message);
    }
}

void WindowMyComputer::OnTabKey(wxKeyEvent &event)
{
    if (event.GetKeyCode() == WXK_TAB && !event.HasModifiers())
    {
        FilePanel *newActive = (m_activePanel == m_leftPanel) ? m_rightPanel : m_leftPanel;
        SetActivePanel(newActive);
        newActive->GetFileList()->SetFocus(); // Явно устанавливаем фокус на список
        return; // Не пропускаем событие дальше
    }
    event.Skip();
}

void WindowMyComputer::OnClose(wxCommandEvent & /*event*/)
{
    CloseModal();
}

void WindowMyComputer::OnStatusUpdate(wxCommandEvent &event)
{
    UpdateStatus(event.GetString());
}

void WindowMyComputer::OnPanelActivate(wxCommandEvent &event)
{
    FilePanel *panel = dynamic_cast<FilePanel *>(FindWindowById(event.GetId()));
    if (panel && (panel == m_leftPanel || panel == m_rightPanel))
    {
        SetActivePanel(panel);
    }
}
