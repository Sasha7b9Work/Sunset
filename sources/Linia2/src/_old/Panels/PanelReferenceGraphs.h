// 2025/7/26 23:57:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


// Архив эталонных графиков


class MyGrid : public wxGrid
{
public:
    MyGrid(wxWindow *);
};


class PanelReferenceGraph : public Panel
{
public:

    PanelReferenceGraph(wxWindow *);

    static PanelReferenceGraph *self;

private:

    wxMenuItem *itemClear = nullptr,
        *itemDeleteFrom = nullptr,
        *itemLoadFrom = nullptr,
        *itemReturn = nullptr;

    wxButton *btnReturn = nullptr;

    MyGrid *grid = nullptr;

    struct Line
    {
        int        number;
        wxString   graphic;
        wxDateTime time;
        wxString   comment;
    };

    wxVector <Line> lines;

    wxArrayString titles_columns;

    void OnEventButton(wxCommandEvent &);
    void OnEventRightClick(wxMouseEvent &);
    void OnEventMenu(wxCommandEvent &);
    void OnEventGridClick(wxGridEvent &);

    void SetTitlesColumn();

    void SetAutoSizeColumns();

    void CreateTestLines();

    void UpdateLines();
};
