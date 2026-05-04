// 2026/03/11 09:57:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/gdicmn.h>
#pragma warning(pop)


// В случае какого-либо события (например, изменили схему включения), вызывается соотвествующая этому событию функция,
// чтобы программа произвела необходимые действия (например, изменила подписи осей графиков)
namespace Events
{
    // Следует вызывает при изменении испытуемого элемент или способе его подключения
    void ChangingTheConnectionOfTheMeasuredElement();

    // Пользователь сместил мышкой изображение сигнала на экране
    void ChangeOffsetMeasure(const wxPoint &);
}
