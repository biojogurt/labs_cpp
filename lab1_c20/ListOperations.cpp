#include "ListOperations.h"
#include "NumberOperations.h"
#include <algorithm>

double ListOperations::choose_modified_number(int number, double average, int &positive_count)
{
    if (number > 0)
        positive_count++;

    if (positive_count != 3)
        return number;

    positive_count = 0;
    return average;
}

int ListOperations::numbers_sum(std::list<int>::iterator begin, std::list<int>::iterator end)
{
    int sum = 0;

    for (std::list<int>::iterator i = begin; i != end; i++)
        sum += *i;

    return sum;
}

double ListOperations::numbers_average(std::list<int>::iterator begin, std::list<int>::iterator end)
{
    return static_cast<double> (numbers_sum(begin, end)) / std::distance(begin, end);
}

std::list<int> ListOperations::get_numbers_from_file(std::wfstream &file)
{
    file.clear();
    file.seekp(0, std::ios_base::beg);

    std::list<int> numbers;
    std::wstring number;

    while (std::getline(file, number))
    {
        if (!NumberOperations::is_number(number))
            throw std::ios_base::failure("Not a number encountered");

        numbers.push_back(std::stoi(number));
    }

    return numbers;
}

std::list<double> ListOperations::modify(std::list<int> &numbers)
{
    std::list<double> modified_numbers;
    double average = numbers_average(numbers);
    int positive_count = 0;

    for (int number : numbers)
        modified_numbers.push_back(choose_modified_number(number, average, positive_count));

    return modified_numbers;
}

std::list<double> ListOperations::modify(std::list<int>::iterator begin, std::list<int>::iterator end)
{
    std::list<double> modified_numbers;
    double average = numbers_average(begin, end);
    int positive_count = 0;

    for (std::list<int>::iterator i = begin; i != end; i++)
        modified_numbers.push_back(choose_modified_number(*i, average, positive_count));

    return modified_numbers;
}

std::list<double> ListOperations::modify_transform(std::list<int> &numbers)
{
    std::list<double> modified_numbers;
    double average = numbers_average(numbers);
    int positive_count = 0;

    std::transform(numbers.begin(), numbers.end(), std::back_inserter(modified_numbers), [average, &positive_count](int number)
                   { return choose_modified_number(number, average, positive_count); });

    return modified_numbers;
}

std::list<double> ListOperations::modify_transform(std::list<int>::iterator begin, std::list<int>::iterator end)
{
    std::list<double> modified_numbers;
    double average = numbers_average(begin, end);
    int positive_count = 0;

    std::transform(begin, end, std::back_inserter(modified_numbers), [average, &positive_count](int number)
                   { return choose_modified_number(number, average, positive_count); });

    return modified_numbers;
}

std::list<double> ListOperations::modify_for_each(std::list<int> &numbers)
{
    std::list<double> modified_numbers;
    double average = numbers_average(numbers);
    int positive_count = 0;

    std::for_each(numbers.begin(), numbers.end(), [average, &positive_count, &modified_numbers](int number)
                  { modified_numbers.push_back(choose_modified_number(number, average, positive_count)); });

    return modified_numbers;
}

std::list<double> ListOperations::modify_for_each(std::list<int>::iterator begin, std::list<int>::iterator end)
{
    std::list<double> modified_numbers;
    double average = numbers_average(begin, end);
    int positive_count = 0;

    std::for_each(begin, end, [average, &positive_count, &modified_numbers](int number)
                  { modified_numbers.push_back(choose_modified_number(number, average, positive_count)); });

    return modified_numbers;
}

int ListOperations::numbers_sum(const std::list<int> &numbers)
{
    int sum = 0;

    for (int number : numbers)
        sum += number;

    return sum;
}

double ListOperations::numbers_average(const std::list<int> &numbers)
{
    return static_cast<double> (numbers_sum(numbers)) / numbers.size();
}