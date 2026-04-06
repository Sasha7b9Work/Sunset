// 2025/09/07 19:41:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

// Здесь находятся тесты классов и функций

namespace SoftTests
{
    bool RunAll();
}


// Тесты сущностей каталога Data
namespace DataTests
{
    bool RunAll();

    bool Converter();
}


namespace SCPITests
{
    bool RunAll();
}
