#pragma once
#include <fstream>
#include <list>

class ListOperations
{
private:
    double static choose_modified_number(int, double, int &);

    int static numbers_sum(std::list<int>::iterator, std::list<int>::iterator);
    double static numbers_average(std::list<int>::iterator, std::list<int>::iterator);

public:
    std::list<int> static get_numbers_from_file(std::wfstream &);

    std::list<double> static modify(std::list<int> &);
    std::list<double> static modify(std::list<int>::iterator, std::list<int>::iterator);

    std::list<double> static modify_transform(std::list<int> &);
    std::list<double> static modify_transform(std::list<int>::iterator, std::list<int>::iterator);

    std::list<double> static modify_for_each(std::list<int> &);
    std::list<double> static modify_for_each(std::list<int>::iterator, std::list<int>::iterator);

    int static numbers_sum(const std::list<int> &);
    double static numbers_average(const std::list<int> &);

    template<class T>
    void static print(std::wostream &, const std::list<T> &);
};

template<class T>
void ListOperations::print(std::wostream &os, const std::list<T> &numbers)
{
    for (T number : numbers)
        os << number << L"\n";
}