// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/StringUtils.h"
#pragma warning(push, 0)
#include <wx/gdicmn.h>
#pragma warning(pop)
#include <cstring>


wxSize BUTTON_SIZE = { BUTTON_WIDTH, BUTTON_HEIGHT };
wxSize BUTTON_SIZE_SMALL = { 100, BUTTON_HEIGHT };


wxString BitSet128::ToASCII() const
{
    return wxString::Format("%08X%08X%08X%08X", word[3], word[2], word[1], word[0]);
}


void BitSet128::FromASCII(const wxString &input)
{
    if (input.length() <= 16)
    {
        word64[1] = 0;

        word64[0] = SU::UInt64FromHex(input);
    }
    else
    {
        char str_value[128];
        std::strcpy(str_value, input.c_str());

        char *end = str_value + std::strlen(str_value);

        word64[0] = SU::UInt64FromHex(end - 16);

//        end -= 15;
        *end = '\0';

        word64[1] = SU::UInt64FromHex(str_value);
    }
}


bool BitSet128::operator==(const BitSet128 &rhs) const
{
    return std::memcmp(this, &rhs, sizeof(rhs)) == 0;
}


bool BitSet128::operator!=(const BitSet128 &rhs) const
{
    return std::memcmp(this, &rhs, sizeof(rhs)) != 0;
}
