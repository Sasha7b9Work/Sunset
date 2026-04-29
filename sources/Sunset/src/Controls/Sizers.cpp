// 2026/04/23 11:53:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Sizers.h"


SizerVert::SizerVert() : wxBoxSizer(wxVERTICAL)
{

}


void SizerVert::AddSizer(wxBoxSizer *sizer)
{
    AddSpacer(spacer);

    wxBoxSizer::Add(sizer, 0, wxEXPAND | wxLEFT | wxRIGHT);
}


void SizerVert::AddWidgetsInSizer(wxWindow *widget1, wxWindow *widget2)
{
    SizerHor *sizer = new SizerHor();

    sizer->AddWidget(widget1);

    if (widget2)
    {
        sizer->AddWidget(widget2);
    }

    sizer->Finish();

    AddSizer(sizer);
}


SizerHor::SizerHor() : wxBoxSizer(wxHORIZONTAL)
{
    AddSpacer(spacer);
}


void SizerHor::AddWidget(wxWindow *widget)
{
    wxBoxSizer::Add(widget, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

    AddSpacer(spacer);
}


void SizerHor::Finish()
{
    AddStretchSpacer();
}
