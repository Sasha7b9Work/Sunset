// 2025/6/1 18:08:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Panels/PanelTable.h"
#include "Panels/PanelMenu/PanelMenu.h"
#include "MainWindow.h"


PanelTable *PanelTable::self = nullptr;


PanelTable::PanelTable(wxWindow* parent) :
    Panel(parent, 0, MainWindow::HEIGHT_DRAW - HEIGHT, WIDTH, HEIGHT)
{
    self = this;

    {
        titles_columns.Add(wxT("Точка"));
        titles_columns.Add("Uc, V");
        titles_columns.Add("Ic, A");
        titles_columns.Add("Ib, A");
        titles_columns.Add(" ");
    }

    grid = new wxGrid(this, wxID_ANY);

    grid->EnableEditing(false);
    grid->EnableGridLines(false);
    grid->EnableDragCell(false);
    grid->EnableCellEditControl(false);

    grid->SetRowLabelSize(0);

    grid->CreateGrid(0, 5);

    grid->SetColLabelSize(20);

    SetTitlesColumn();

    grid->SetScrollRate(10, 10);

    grid->AutoSizeColumns();

    // Размещаем grid в sizer для правильного масштабирования
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(grid, 1, wxEXPAND | wxALL, 5);
    SetSizer(sizer);

    Panel::Layout();

    SetAutoSizeColumns();

    {
        lines.push_back({ 1, 1, 1, 0.0f, 0.0f, 0.0f });
        lines.push_back({ 2, 1, 2, 0.0f, 0.0f, 0.0f });
        lines.push_back({ 3, 1, 3, 0.0f, 0.0f, 0.1f });
        lines.push_back({ 4, 1, 4, 0.0f, 0.0f, 0.05f });
    }

    UpdateLines();

    grid->SetSelectionMode(wxGrid::wxGridSelectNone);

    grid->SetCellHighlightPenWidth(0);
    grid->SetCellHighlightROPenWidth(0);
}


void PanelTable::SetTitlesColumn()
{
    for (uint i = 0; i < 5; i++)
    {
        grid->SetColLabelValue((int)i, titles_columns[i]);
    }
}


void PanelTable::SetAutoSizeColumns()
{
    int gridWidth = grid->GetClientSize().GetWidth();
    int colsCount = grid->GetNumberCols();
    int colWidth = (gridWidth - grid->GetRowLabelSize()) / colsCount - 5;

    for (int col = 0; col < colsCount; ++col)
    {
        grid->SetColSize(col, colWidth);
    }
}


void PanelTable::UpdateLines()
{
    if (grid->GetNumberRows() < (int)lines.size())
    {
        grid->AppendRows((int)lines.size() - grid->GetNumberRows());
    }

    for (uint i = 0; i < lines.size(); i++)
    {
        const Line &l = lines[i];

        grid->SetCellValue((int)i, 0, wxString::Format("%d/%d/%d", l.p1, l.p2, l.p3));

        InsertValue((int)i, 1, l.Uc);

        InsertValue((int)i, 2, l.Ic);

        InsertValue((int)i, 3, l.Ib);
    }
}


void PanelTable::InsertValue(int row, int col, float value)
{
    if (value == 0.0f)
    {
        grid->SetCellValue(row, col, "0");
    }
    else
    {
        grid->SetCellValue(row, col, wxString::Format("%.3f", value));  
    }
}
