#pragma once
#include "ListOperations.h"
#include <iostream>

class MenuOperations
{
private:
    int static choose_operation(int);

    int static input_number();
    int static input_positive_number();
    int static input_number_in_range(int, int);

    std::list<double> static modify_no_range(std::list<int> &, int);
    std::list<double> static modify_range(std::list<int> &, int);

    bool static choose_file();

    template<class T>
    void static print_numbers_to_screen(const std::list<T> &);

    void static modify(std::list<int> &, std::list<double> &);
    void static print_sum(const std::list<int> &);
    void static print_average(const std::list<int> &);
    bool static original_list_menu(std::list<int> &, std::list<double> &, std::wstring);

    void static print_numbers_to_file(const std::list<double> &);
    bool static modified_list_menu(const std::list<double> &);

    void static fill_file_with_numbers(std::wfstream &);
    void static get_numbers_from_file(std::wfstream &, std::list<int> &);
    int static display_main_menu(int, int, std::wstring &);

public:
    bool static main_menu(std::wfstream &, std::list<int> &, std::list<double> &);
};

template<class T>
void MenuOperations::print_numbers_to_screen(const std::list<T> &numbers)
{
    std::wcout << L"\n  Числа в списке:\n\n";
    ListOperations::print(std::wcout, numbers);
}