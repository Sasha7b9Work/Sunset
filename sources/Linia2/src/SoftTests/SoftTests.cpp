// 2025/09/07 19:43:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SoftTests/SoftTests.h"


bool SoftTests::RunAll()
{
    return DataTests::RunAll() &&
        SCPITests::RunAll();
}

