// 2026/04/03 11:48:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DescriptionControl
{
    DescriptionControl(void **_pointer) : pointer(_pointer) { }

    virtual void Create(wxWindow *) = 0;

    virtual ~DescriptionControl() = default;

    void **pointer = nullptr;                    // Здесь будет храниться указатель на контрол после создания
};


struct DescRadioButton : public DescriptionControl
{
    wxString name;
    int flags;

    DescRadioButton(wxRadioButton **_pointer, const wxString &_name, int _flags = 0) :
        DescriptionControl((void **)_pointer),
        name(_name), flags(_flags) { }

    virtual void Create(wxWindow *) override;
};
