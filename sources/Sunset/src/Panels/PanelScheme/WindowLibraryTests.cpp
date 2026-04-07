// 2025/7/24 22:48:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelScheme/WindowLibraryTests.h"
#include "Utils/SystemDepend.h"
#include "Controls/Dialog.h"
#include "Controls/StaticBox.h"


WindowLibraryTests::WindowLibraryTests() :
    Dialog(wxT("Библиотека пользовательских тестов"), wxDefaultPosition, {WIDTH, HEIGHT})
{

    int dw = 150;

    {
        int width = WIDTH - dw;

        wxSize size{ width, HEIGHT };

        wxPanel *leftPanel = new wxPanel(this, wxID_ANY, { 0, 0 }, size);

        leftPanel->SetSizeHints(width, HEIGHT, width, HEIGHT);

        grid = new wxGrid(leftPanel, wxID_ANY);
        grid->SetRowLabelSize(0);
        grid->CreateGrid(0, 3);
        SetTitlesColumn();
        grid->EnableEditing(true);
        grid->SetScrollRate(10, 10);

        grid->AutoSizeColumns();

        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(grid, 1, wxEXPAND | wxALL, 5);
        leftPanel->SetSizer(sizer);

        leftPanel->Layout();

        grid->EnableEditing(false);
        grid->EnableGridLines(false);
        grid->EnableDragCell(false);
        grid->EnableCellEditControl(false);
        grid->SetSelectionMode(wxGrid::wxGridSelectRows);
        grid->SetCellHighlightPenWidth(0);
        grid->SetCellHighlightROPenWidth(0);
    }

    {
        wxSize size{ dw, HEIGHT };

        wxPanel *rightPanel = new wxPanel(this, wxID_ANY, { WIDTH - dw, 0 }, size);

        StaticBox *box1 = new StaticBox(rightPanel, "Категория", { 0, 0 }, {size.x - 20, 60});

        {
            wxArrayString choices;
            choices.Add("NMOS");

            new ButtonsCombo(box1, "", { 10, SD::Y_SB(25) }, 60, choices, choices, 1, "");
        }

        StaticBox *box2 = new StaticBox(rightPanel, "Тест", { 0, box1->GetPosition().y + box1->GetSize().y + 5 }, { size.x - 20, 145 });

        {
            int y0 = 25;

            int dy = 40;

            new wxButton(box2, wxID_ANY, "Загрузить", { 10, SD::Y_SB(y0) }, { 100, BUTTON_HEIGHT });
            new wxButton(box2, wxID_ANY, "Удалить", { 10, SD::Y_SB(y0 + dy) }, { 100, BUTTON_HEIGHT });
            new wxButton(box2, wxID_ANY, "Удалить все", { 10, SD::Y_SB(y0 + dy * 2) }, { 100, BUTTON_HEIGHT });
        }
    }

    // Устанавливаем минимальный размер окна
    Dialog::SetMinSize(wxSize(WIDTH, HEIGHT));

    Dialog::SetSizeHints(WIDTH, HEIGHT, WIDTH, HEIGHT);

    Dialog::Layout();

    SetAutoSizeColumns();

    Fill();
}


void WindowLibraryTests::SetTitlesColumn()
{
    grid->SetColLabelValue(0, "#");
    grid->SetColLabelValue(1, "Наименование файла");
    grid->SetColLabelValue(2, "Комментарий");
}


void WindowLibraryTests::SetAutoSizeColumns()
{
    int gridWidth = grid->GetClientSize().GetWidth();
    int colsCount = grid->GetNumberCols();
    int colWidth = (gridWidth - grid->GetRowLabelSize()) / colsCount;

    for (int col = 0; col < colsCount; ++col)
    {
        grid->SetColSize(col, colWidth);
    }
}


void WindowLibraryTests::Fill()
{
    AppendLine("1", "IdVd_IRF840.tst", wxT("Выходная характеристика"));
}


void WindowLibraryTests::AppendLine(const wxString &c0, const wxString &c1, const wxString &c2)
{
    grid->AppendRows(1);

    int row = grid->GetNumberRows() - 1;

    grid->SetCellValue(row, 0, c0);
    grid->SetCellValue(row, 1, c1);
    grid->SetCellValue(row, 2, c2);
}
