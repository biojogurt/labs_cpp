#define NOMINMAX
#include "MenuOperations.h"
#include "FileOperations.h"
#include "ListOperations.h"
#include "NumberOperations.h"
#include <windows.h>

int MenuOperations::choose_operation(int choicesAmount)
{
    std::wcout << L"\x1b[?25l\x1b[" + std::to_wstring(choicesAmount) + L"A";
    int position = 1;
    while (true)
    {
        INPUT_RECORD keys[128];
        DWORD keysRead;
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), keys, 128, &keysRead);
        for (DWORD i = 0; i < keysRead; i++)
        {
            switch (keys[i].Event.KeyEvent.wVirtualKeyCode)
            {
                case VK_DOWN:
                    if (keys[i].Event.KeyEvent.bKeyDown && position < choicesAmount)
                    {
                        std::wcout << " \r\x1b[B>\r";
                        position++;
                    }
                    break;
                case VK_UP:
                    if (keys[i].Event.KeyEvent.bKeyDown && position > 1)
                    {
                        std::wcout << " \r\x1b[A>\r";
                        position--;
                    }
                    break;
                case VK_RETURN:
                    if (keys[i].Event.KeyEvent.bKeyDown)
                    {
                        std::wcout << L"\x1b[" + std::to_wstring(choicesAmount + 1 - position) + L"B\x1b[?25h";
                        return position;
                    }
            }
        }
    }
}

int MenuOperations::input_number()
{
    std::wstring number;
    std::getline(std::wcin, number);

    while (!NumberOperations::is_number(number))
    {
        std::wcout << L"  Неверный ввод. Пожалуйста, введите число: ";
        std::getline(std::wcin, number);
    }

    return std::stoi(number);
}

int MenuOperations::input_positive_number()
{
    int number = input_number();

    while (number < 1)
    {
        std::wcout << L"  Неверный ввод. Пожалуйста, введите положительное число: ";
        number = input_number();
    }

    return number;
}

int MenuOperations::input_number_in_range(int min, int max)
{
    int number = input_number();

    while (number < min || number > max)
    {
        std::wcout << L"  Неверный ввод. Пожалуйста, введите число не меньше " << min << L" и не больше " << max << L": ";
        number = input_number();
    }

    return number;
}

std::list<double> MenuOperations::modify_no_range(std::list<int> &numbers, int algorithm_choice)
{
    switch (algorithm_choice)
    {
        case 1:
            return ListOperations::modify(numbers);

        case 2:
            return ListOperations::modify_transform(numbers);

        case 3:
            return ListOperations::modify_for_each(numbers);
    }
}

std::list<double> MenuOperations::modify_range(std::list<int> &numbers, int algorithm_choice)
{
    std::wcout << L"\n  Введите номер первого элемента от 1 до " << numbers.size() << L": ";
    int begin = input_number_in_range(1, numbers.size());

    std::wcout << L"\n  Введите номер последнего элемента от " << begin << L" до " << numbers.size() << L": ";
    int end = input_number_in_range(begin, numbers.size());

    std::list<int>::iterator begin_iterator = std::next(numbers.begin(), begin - 1), end_iterator = std::next(numbers.begin(), end);

    switch (algorithm_choice)
    {
        case 1:
            return ListOperations::modify(begin_iterator, end_iterator);

        case 2:
            return ListOperations::modify_transform(begin_iterator, end_iterator);

        case 3:
            return ListOperations::modify_for_each(begin_iterator, end_iterator);
    }
}

bool MenuOperations::choose_file()
{
    std::wcout << L"\n  Хотите ли Вы использовать ранее заполненный файл?\n> да\n  нет\n";
    return (choose_operation(2) == 1);
}

void MenuOperations::modify(std::list<int> &numbers, std::list<double> &modified_numbers)
{
    std::wcout << L"\n  Какой алгоритм Вы хотите использовать?\n> цикл\n  std::transform\n  std::for_each\n";
    int algorithm_choice = choose_operation(3);

    std::wcout << L"\n  Хотите ли Вы задать промежуток для обработки?\n> обработать весь список\n  задать промежуток\n";
    switch (choose_operation(2))
    {
        case 1:
            modified_numbers = modify_no_range(numbers, algorithm_choice);
            break;

        case 2:
            modified_numbers = modify_range(numbers, algorithm_choice);
            break;
    }

    std::wcout << L"\n  Новый список создан.\n";
}

void MenuOperations::print_sum(const std::list<int> &numbers)
{
    std::wcout << L"\n  Сумма: " << ListOperations::numbers_sum(numbers) << L"\n";
}

void MenuOperations::print_average(const std::list<int> &numbers)
{
    std::wcout << L"\n  Среднее арифметическое: " << ListOperations::numbers_average(numbers) << L"\n";
}

bool MenuOperations::original_list_menu(std::list<int> &numbers, std::list<double> &modified_numbers, std::wstring original_list_name)
{
    std::wcout << L"\n  ---Операции работы " << original_list_name << L"---\n"
        << L"> заменить каждое третье положительное число в списке средним арифметическим всех чисел (новый список)\n  вычислить сумму чисел в списке\n"
        << L"  вычислить среднее арифметическое чисел в списке\n  вывести числа в списке на экран\n  назад\n";

    switch (choose_operation(5))
    {
        case 1:
            modify(numbers, modified_numbers);
            break;

        case 2:
            print_sum(numbers);
            break;

        case 3:
            print_average(numbers);
            break;

        case 4:
            print_numbers_to_screen(numbers);
            break;

        case 5:
            std::wcout << L"\n";
            return false;
    }

    return true;
}

void MenuOperations::print_numbers_to_file(const std::list<double> &numbers)
{
    std::wcout << L"\n  Введите имя файла: ";
    std::wstring filename = FileOperations::input_filename();
    std::wfstream file;

    try
    {
        file = FileOperations::open_file(filename);
    }
    catch (const std::ios_base::failure &)
    {
        std::wcout << L"\n  Файл с таким названием невозможно создать.\n";
        return;
    }

    ListOperations::print(file, numbers);
    std::wcout << L"\n  Числа выведены в файл.\n";
    file.close();
}

bool MenuOperations::modified_list_menu(const std::list<double> &modified_numbers)
{
    std::wcout << L"\n  ---Операции работы с модифицированным списком---\n"
        << L"> вывести числа в списке на экран\n  вывести числа в списке в файл\n  назад\n";

    switch (choose_operation(3))
    {
        case 1:
            print_numbers_to_screen(modified_numbers);
            break;

        case 2:
            print_numbers_to_file(modified_numbers);
            break;

        case 3:
            std::wcout << L"\n";
            return false;
    }

    return true;
}

void MenuOperations::fill_file_with_numbers(std::wfstream &file)
{
    if (file.is_open())
        file.close();

    std::wcout << L"\n  Введите N: ";
    int n = input_positive_number();

    std::wcout << L"  Введите M: ";
    int m = input_positive_number();

    std::wcout << L"  Введите имя файла: ";
    std::wstring filename = FileOperations::input_filename();

    std::wcout << L"  Какой алгоритм Вы хотите использовать?\n> цикл\n  std::generate\n";
    bool using_generate = (choose_operation(2) == 2);

    try
    {
        file = FileOperations::fill_file_with_numbers(filename, n, m, using_generate);
        std::wcout << L"\n  Файл создан.\n\n";
    }
    catch (const std::ios_base::failure &)
    {
        std::wcout << L"\n  Файл с таким названием невозможно создать.\n\n";
    }
}

void MenuOperations::get_numbers_from_file(std::wfstream &file, std::list<int> &numbers)
{
    if (file.is_open() && choose_file())
    {
        numbers = ListOperations::get_numbers_from_file(file);
        std::wcout << L"\n  Список создан.\n\n";
        return;
    }

    std::wcout << L"\n  Введите имя файла: ";
    std::wstring filename = FileOperations::input_filename();
    std::wfstream different_file;

    try
    {
        different_file = FileOperations::open_file(filename, std::ios::in | std::ios::out);
    }
    catch (const std::ios_base::failure &)
    {
        std::wcout << L"\n  Файл с таким названием невозможно открыть.\n\n";
        return;
    }

    try
    {
        numbers = ListOperations::get_numbers_from_file(different_file);
    }
    catch (const std::ios_base::failure &)
    {
        std::wcout << L"\n  Файл неправильно оформлен, или в нем содержатся не только цифры. Список не был создан.\n\n";
        return;
    }

    std::wcout << L"\n  Список создан.\n\n";
    different_file.close();
}

int MenuOperations::display_main_menu(int numbers_size, int modified_numbers_size, std::wstring &original_list_name)
{
    std::wcout << L"  ---Главное меню---\n";

    int choices = 3;
    std::wcout << L"> заполнить текстовый файл N целыми случайными числами в диапазоне от -M до M\n  заполнить список числами из файла\n";

    if (numbers_size && !modified_numbers_size)
    {
        choices = 4;
        std::wcout << L"  операции работы со списком\n";
        original_list_name = L"со списком";
    }

    if (modified_numbers_size)
    {
        choices = 5;
        std::wcout << L"  операции работы с оригинальным списком\n  операции работы с модифицированным списком\n";
        original_list_name = L"с оригинальным списком";
    }

    std::wcout << L"  выйти\n";
    return choices;
}

bool MenuOperations::main_menu(std::wfstream &file, std::list<int> &numbers, std::list<double> &modified_numbers)
{
    std::wstring original_list_name;

    switch (choose_operation(display_main_menu(numbers.size(), modified_numbers.size(), original_list_name)))
    {
        case 1:
            fill_file_with_numbers(file);
            break;

        case 2:
            if (modified_numbers.size())
                modified_numbers.clear();

            get_numbers_from_file(file, numbers);
            break;

        case 3:
            if (!numbers.size())
                return false;

            while (original_list_menu(numbers, modified_numbers, original_list_name));
            break;

        case 4:
            if (!modified_numbers.size())
                return false;

            while (modified_list_menu(modified_numbers));
            break;

        case 5:
            return false;
    }

    return true;
}