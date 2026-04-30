// 2026/04/03 11:48:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/radiobut.h>
#pragma warning(pop)


struct DescriptionControl
{
    DescriptionControl(void **_pointer) : pointer(_pointer) { }

    virtual void Create(wxWindow *) = 0;

    virtual ~DescriptionControl() = default;

    void **pointer = nullptr;                    // Здесь будет храниться указатель на контрол после создания
};
