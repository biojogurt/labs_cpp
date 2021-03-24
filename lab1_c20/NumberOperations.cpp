#include "NumberOperations.h"

bool NumberOperations::is_number(std::wstring number)
{
    if (number == L"")
        return false;

    int i = 0;

    if (number[0] == L'-')
        i++;

    for (; i != number.size(); i++)
        if (!iswdigit(number[i]))
            return false;

    return true;
}

int NumberOperations::generate_random_number(int range)
{
    return rand() % (2 * range + 1) - range;
}