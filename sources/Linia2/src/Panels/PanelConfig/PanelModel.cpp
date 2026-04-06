// 2025/6/1 17:41:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelModel.h"
#include "Utils/GlobalFunctions.h"
#include "IPPP/Tests/Model.h"
#include "Panels/PanelConfig/PanelConfig.h"


PanelModel *ThePanelModel = nullptr;


PanelModel::PanelModel(wxWindow* parent, int /*_x*/, int _w, int /*_h*/) :
    wxPanel(parent)
{
    wxPanel::SetName("PanelModel");

    ThePanelModel = this;

    wxPanel::SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - PanelConfig::HEIGHT_BUTTONS });
    wxPanel::SetPosition({ 0, PanelConfig::HEIGHT_BUTTONS });

    const int height_name = 20;

    wxPanel *panel_name = new wxPanel(this, wxID_ANY, { 0, 0 }, { _w, height_name }, wxTAB_TRAVERSAL | wxSUNKEN_BORDER);

    txtName = new wxStaticText(panel_name, wxID_ANY, "", { 0, 0 }, { _w, height_name }, wxALIGN_CENTER);

    panel_graph = new wxPanel(this, wxID_ANY, { 0, height_name }, { _w, HEIGHT - height_name }, wxTAB_TRAVERSAL | wxSUNKEN_BORDER);

    panel_graph->SetBackgroundColour(*wxWHITE);
}


void PanelModel::SetName(const wxString &_name)
{
    txtName->SetLabel(_name);
}


void PanelModel::PeriodicTask()
{
    if (Model::IsEmpty())
    {
        SetName(wxT("Файл модели"));
    }
    else
    {
        SetName(Model::GetName());
    }
}
