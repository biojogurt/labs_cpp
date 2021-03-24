#include "lab1_c20.h"
#include "menu.h"
#include <fcntl.h>
#include <io.h>
#include <algorithm>
#include <iostream>

std::wstring input_filename()
{
    std::wstring filename;
    std::getline(std::wcin, filename);
    return filename;
}

std::wfstream open_file(const std::wstring &filename, std::ios::openmode mode)
{
    std::wfstream file;
    file.open(filename, mode);

    if (file.fail())
        throw std::ios_base::failure("Can't open/create file");

    return file;
}

bool is_number(const std::wstring &number)
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

int input_number()
{
    std::wstring number;
    std::getline(std::wcin, number);

    while (!is_number(number))
    {
        std::wcout << L"  Неверный ввод. Пожалуйста, введите число: ";
        std::getline(std::wcin, number);
    }

    return std::stoi(number);
}

int input_positive_number()
{
    int number = input_number();

    while (number < 1)
    {
        std::wcout << L"  Неверный ввод. Пожалуйста, введите положительное число: ";
        number = input_number();
    }

    return number;
}

int input_number_in_range(int min, int max)
{
    int number = input_number();

    while (number < min || number > max)
    {
        std::wcout << L"  Неверный ввод. Пожалуйста, введите число не меньше " << min << L" и не больше " << max << L": ";
        number = input_number();
    }

    return number;
}

int generate_random_number(int range)
{
    return rand() % (2 * range + 1) - range;
}

double choose_modified_number(int number, double average, int &positive_count)
{
    if (number > 0)
        positive_count++;

    if (positive_count != 3)
        return number;

    positive_count = 0;
    return average;
}

std::wfstream fill_file_with_numbers(const std::wstring &filename, int n, int m, bool using_generate)
{
    std::wfstream file;

    try
    {
        file = open_file(filename);
    }
    catch (std::ios_base::failure &)
    {
        throw;
    }

    std::list<int> numbers(n);

    if (using_generate)
        std::generate(numbers.begin(), numbers.end(), [m] { return generate_random_number(m); });
    else
        for (int &number : numbers)
            number = generate_random_number(m);

    print(file, numbers);
    return file;
}

std::list<int> get_numbers_from_file(std::wfstream &file)
{
    file.clear();
    file.seekp(0, std::ios_base::beg);

    std::list<int> numbers;
    std::wstring number;

    while (std::getline(file, number))
    {
        if (!is_number(number))
            throw std::ios_base::failure("Not a number encountered");

        numbers.push_back(std::stoi(number));
    }

    return numbers;
}

std::list<double> modify(std::list<int> &numbers)
{
    std::list<double> modified_numbers;
    double average = numbers_average(numbers);
    int positive_count = 0;

    for (int number : numbers)
        modified_numbers.push_back(choose_modified_number(number, average, positive_count));

    return modified_numbers;
}

std::list<double> modify(std::list<int>::iterator begin, std::list<int>::iterator end)
{
    std::list<double> modified_numbers;
    double average = numbers_average(begin, end);
    int positive_count = 0;

    for (std::list<int>::iterator i = begin; i != end; i++)
        modified_numbers.push_back(choose_modified_number(*i, average, positive_count));

    return modified_numbers;
}

std::list<double> modify_transform(std::list<int> &numbers)
{
    std::list<double> modified_numbers;
    double average = numbers_average(numbers);
    int positive_count = 0;

    std::transform(numbers.begin(), numbers.end(), std::back_inserter(modified_numbers), [average, &positive_count](int number)
                   { return choose_modified_number(number, average, positive_count); });

    return modified_numbers;
}

std::list<double> modify_transform(std::list<int>::iterator begin, std::list<int>::iterator end)
{
    std::list<double> modified_numbers;
    double average = numbers_average(begin, end);
    int positive_count = 0;

    std::transform(begin, end, std::back_inserter(modified_numbers), [average, &positive_count](int number)
                   { return choose_modified_number(number, average, positive_count); });

    return modified_numbers;
}

std::list<double> modify_for_each(std::list<int> &numbers)
{
    std::list<double> modified_numbers;
    double average = numbers_average(numbers);
    int positive_count = 0;

    std::for_each(numbers.begin(), numbers.end(), [average, &positive_count, &modified_numbers](int number)
                  { modified_numbers.push_back(choose_modified_number(number, average, positive_count)); });

    return modified_numbers;
}

std::list<double> modify_for_each(std::list<int>::iterator begin, std::list<int>::iterator end)
{
    std::list<double> modified_numbers;
    double average = numbers_average(begin, end);
    int positive_count = 0;

    std::for_each(begin, end, [average, &positive_count, &modified_numbers](int number)
                  { modified_numbers.push_back(choose_modified_number(number, average, positive_count)); });

    return modified_numbers;
}

int numbers_sum(const std::list<int> &numbers)
{
    int sum = 0;

    for (int number : numbers)
        sum += number;

    return sum;
}

int numbers_sum(std::list<int>::iterator begin, std::list<int>::iterator end)
{
    int sum = 0;

    for (std::list<int>::iterator i = begin; i != end; i++)
        sum += *i;

    return sum;
}

double numbers_average(const std::list<int> &numbers)
{
    return static_cast<double> (numbers_sum(numbers)) / numbers.size();
}

double numbers_average(std::list<int>::iterator begin, std::list<int>::iterator end)
{
    return static_cast<double> (numbers_sum(begin, end)) / std::distance(begin, end);
}

template<class T>
void print(std::wostream &os, const std::list<T> &numbers)
{
    for (T number : numbers)
        os << number << L"\n";
}

void display_fill_file_with_numbers(std::wfstream &file)
{
    if (file.is_open())
        file.close();

    std::wcout << L"\n  Введите N: ";
    int n = input_positive_number();

    std::wcout << L"  Введите M: ";
    int m = input_positive_number();

    std::wcout << L"  Введите имя файла: ";
    std::wstring filename = input_filename();

    std::wcout << L"  Какой алгоритм Вы хотите использовать?\n> цикл\n  std::generate\n";
    bool using_generate = (Menu::menu(2) == 2);

    try
    {
        file = fill_file_with_numbers(filename, n, m, using_generate);
        std::wcout << L"\n  Файл создан.\n\n";
    }
    catch (const std::ios_base::failure &)
    {
        std::wcout << L"\n  Файл с таким названием невозможно создать.\n\n";
    }
}

bool display_choose_file()
{
    std::wcout << L"\n  Хотите ли Вы использовать ранее заполненный файл?\n> да\n  нет\n";
    return (Menu::menu(2) == 1);
}

void display_get_numbers_from_file(std::wfstream &file, std::list<int> &numbers)
{
    if (file.is_open() && display_choose_file())
    {
        numbers = get_numbers_from_file(file);
        std::wcout << L"\n  Список создан.\n\n";
        return;
    }

    std::wcout << L"\n  Введите имя файла: ";
    std::wstring filename = input_filename();
    std::wfstream different_file;

    try
    {
        different_file = open_file(filename, std::ios::in | std::ios::out);
    }
    catch (const std::ios_base::failure &)
    {
        std::wcout << L"\n  Файл с таким названием невозможно открыть.\n\n";
        return;
    }

    try
    {
        numbers = get_numbers_from_file(different_file);
    }
    catch (const std::ios_base::failure &)
    {
        std::wcout << L"\n  Файл неправильно оформлен, или в нем содержатся не только цифры. Список не был создан.\n\n";
        return;
    }

    std::wcout << L"\n  Список создан.\n\n";
    different_file.close();
}

std::list<double> modify_no_range(std::list<int> &numbers, int algorithm_choice)
{
    switch (algorithm_choice)
    {
        case 1:
            return modify(numbers);

        case 2:
            return modify_transform(numbers);

        case 3:
            return modify_for_each(numbers);
    }
}

std::list<double> display_modify_range(std::list<int> &numbers, int algorithm_choice)
{
    std::wcout << L"\n  Введите номер первого элемента от 1 до " << numbers.size() << L": ";
    int begin = input_number_in_range(1, numbers.size());

    std::wcout << L"\n  Введите номер последнего элемента от " << begin << L" до " << numbers.size() << L": ";
    int end = input_number_in_range(begin, numbers.size());

    std::list<int>::iterator begin_iterator = std::next(numbers.begin(), begin - 1), end_iterator = std::next(numbers.begin(), end);

    switch (algorithm_choice)
    {
        case 1:
            return modify(begin_iterator, end_iterator);

        case 2:
            return modify_transform(begin_iterator, end_iterator);

        case 3:
            return modify_for_each(begin_iterator, end_iterator);
    }
}

std::list<double> display_modify(std::list<int> &numbers)
{
    std::wcout << L"\n  Какой алгоритм Вы хотите использовать?\n> цикл\n  std::transform\n  std::for_each\n";
    int algorithm_choice = Menu::menu(3);

    std::wcout << L"\n  Хотите ли Вы задать промежуток для обработки?\n> обработать весь список\n  задать промежуток\n";
    switch (Menu::menu(2))
    {
        case 1:
            return modify_no_range(numbers, algorithm_choice);

        case 2:
            return display_modify_range(numbers, algorithm_choice);
    }
}

void display_print_to_file(const std::list<double> &modified_numbers)
{
    std::wcout << L"\n  Введите имя файла: ";
    std::wstring filename = input_filename();
    std::wfstream file;

    try
    {
        file = open_file(filename);
    }
    catch (const std::ios_base::failure &)
    {
        std::wcout << L"\n  Файл с таким названием невозможно создать.\n";
        return;
    }

    print(file, modified_numbers);
    std::wcout << L"\n  Числа выведены в файл.\n";
    file.close();
}

bool display_original_list_menu(std::list<int> &numbers, std::list<double> &modified_numbers, const std::wstring &original_list_name)
{
    std::wcout << L"\n  ---Операции работы " << original_list_name << L"---\n"
        << L"> заменить каждое третье положительное число в списке средним арифметическим всех чисел (новый список)\n  вычислить сумму чисел в списке\n"
        << L"  вычислить среднее арифметическое чисел в списке\n  вывести числа в списке на экран\n  назад\n";

    switch (Menu::menu(5))
    {
        case 1:
            modified_numbers = display_modify(numbers);
            std::wcout << L"\n  Новый список создан.\n";
            break;

        case 2:
            std::wcout << L"\n  Сумма: " << numbers_sum(numbers) << L"\n";
            break;

        case 3:
            std::wcout << L"\n  Среднее арифметическое: " << numbers_average(numbers) << L"\n";
            break;

        case 4:
            std::wcout << L"\n  Числа в списке:\n\n";
            print(std::wcout, numbers);
            break;

        case 5:
            std::wcout << L"\n";
            return false;
    }

    return true;
}

bool display_modified_list_menu(const std::list<double> &modified_numbers)
{
    std::wcout << L"\n  ---Операции работы с модифицированным списком---\n"
        << L"> вывести числа в списке на экран\n  вывести числа в списке в файл\n  назад\n";

    switch (Menu::menu(3))
    {
        case 1:
            std::wcout << L"\n  Числа в списке:\n\n";
            print(std::wcout, modified_numbers);
            break;

        case 2:
            display_print_to_file(modified_numbers);
            break;

        case 3:
            std::wcout << L"\n";
            return false;
    }

    return true;
}

int display_main_menu(const std::list<int> &numbers, const std::list<double> &modified_numbers, std::wstring &original_list_name)
{
    std::wcout << L"  ---Главное меню---\n";

    int choices = 3;
    std::wcout << L"> заполнить текстовый файл N целыми случайными числами в диапазоне от -M до M\n  заполнить список числами из файла\n";

    if (numbers.size() && !modified_numbers.size())
    {
        choices = 4;
        original_list_name = L"со списком";
        std::wcout << L"  операции работы со списком\n";
    }

    if (modified_numbers.size())
    {
        choices = 5;
        original_list_name = L"с оригинальным списком";
        std::wcout << L"  операции работы с оригинальным списком\n  операции работы с модифицированным списком\n";
    }

    std::wcout << L"  выйти\n";
    return choices;
}

bool main_menu(std::wfstream &file, std::list<int> &numbers, std::list<double> &modified_numbers)
{
    std::wstring original_list_name;

    switch (Menu::menu(display_main_menu(numbers, modified_numbers, original_list_name)))
    {
        case 1:
            display_fill_file_with_numbers(file);
            break;

        case 2:
            if (modified_numbers.size())
                modified_numbers.clear();

            display_get_numbers_from_file(file, numbers);
            break;

        case 3:
            if (!numbers.size())
                return false;

            while (display_original_list_menu(numbers, modified_numbers, original_list_name));
            break;

        case 4:
            if (!modified_numbers.size())
                return false;

            while (display_modified_list_menu(modified_numbers));
            break;

        case 5:
            return false;
    }

    return true;
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    srand(std::time(0));

    std::wfstream file;
    std::list<int> numbers;
    std::list<double> modified_numbers;

    while (main_menu(file, numbers, modified_numbers));

    if (file.is_open())
        file.close();
}