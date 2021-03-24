#pragma once
#include <fstream>
#include <string>

class FileOperations
{
public:
    std::wstring static input_filename();
    std::wfstream static open_file(std::wstring, std::ios::openmode = std::ios::in | std::ios::out | std::ios::trunc);

    std::wfstream static fill_file_with_numbers(std::wstring, int, int, bool);
};