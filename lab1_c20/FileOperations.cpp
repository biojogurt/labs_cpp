#include "FileOperations.h"
#include "ListOperations.h"
#include "NumberOperations.h"
#include <algorithm>
#include <iostream>

std::wstring FileOperations::input_filename()
{
    std::wstring filename;
    std::getline(std::wcin, filename);
    return filename;
}

std::wfstream FileOperations::open_file(std::wstring filename, std::ios::openmode mode)
{
    std::wfstream file;
    file.open(filename, mode);

    if (file.fail())
        throw std::ios_base::failure("Can't open/create file");

    return file;
}

std::wfstream FileOperations::fill_file_with_numbers(std::wstring filename, int n, int m, bool using_generate)
{
    std::wfstream file = open_file(filename);
    std::list<int> numbers(n);

    if (using_generate)
        std::generate(numbers.begin(), numbers.end(), [m] { return NumberOperations::generate_random_number(m); });
    else
        for (int &number : numbers)
            number = NumberOperations::generate_random_number(m);

    ListOperations::print(file, numbers);
    return file;
}