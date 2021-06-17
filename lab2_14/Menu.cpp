#define NOMINMAX
#include "Menu.h"
#include <fstream>
#include <windows.h>

int Menu::choose_operation(int choices_amount)
{
    std::wcout << L"\x1b[?25l\x1b[" + std::to_wstring(choices_amount) + L"A";
    int position = 1;

    while (true)
    {
        INPUT_RECORD keys[128];
        DWORD keys_read;
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), keys, 128, &keys_read);

        for (DWORD i = 0; i < keys_read; i++)
        {
            switch (keys[i].Event.KeyEvent.wVirtualKeyCode)
            {
                case VK_DOWN:
                    if (keys[i].Event.KeyEvent.bKeyDown && position < choices_amount)
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
                        std::wcout << L"\x1b[" + std::to_wstring(choices_amount + 1 - position) + L"B\x1b[?25h";
                        return position;
                    }
            }
        }
    }
}

bool Menu::is_number(std::wstring number)
{
    if (number == L"")
        return false;

    for (int i = 0; i != number.size(); i++)
        if (!iswdigit(number[i]))
            return false;

    return true;
}

int Menu::get_number()
{
    std::wstring number;
    std::getline(std::wcin, number);

    while (!is_number(number))
    {
        std::wcout << L"Неверный ввод. Пожалуйста, введите число: ";
        std::getline(std::wcin, number);
    }

    return std::stoi(number);
}

Date Menu::get_date()
{
    while (true)
    {
        std::wstring date;
        std::getline(std::wcin, date);

        try
        {
            return Date(date);
        }
        catch (...)
        {
            std::wcout << L"Неверный ввод. Пожалуйста, введите дату в формате d.m.y: ";
        }
    }
}

void Menu::get_field(Bill &bill, int field)
{
    switch (field)
    {
        case 0:
            std::getline(std::wcin, bill.firm_name);
            break;

        case 1:
            std::getline(std::wcin, bill.work_type);
            break;

        case 2:
            std::getline(std::wcin, bill.unit);
            break;

        case 3:
            bill.work_cost = get_number();
            break;

        case 4:
            bill.completion_date = get_date();
            break;

        case 5:
            bill.work_amount = get_number();
            break;
    }
}

void Menu::display_get_bill_line(bool &first, std::wstring field_name)
{
    if (!first)
        std::wcout << L"  ";
    else
    {
        std::wcout << L"> ";
        first = false;
    }

    std::wcout << field_name;
}

int Menu::display_get_bill(bool fields_done[6], int fields_num[6], int choice)
{
    std::wstring fields_name[6]{L"название фирмы\n", L"вид работ\n", L"единица измерения\n", L"стоимость единицы выполненной работы\n", L"дата исполнения\n", L"объем работ\n"};
    bool first = true;
    int displayed = 0;

    for (int i = 0; i < 6; i++)
        fields_num[i] = i + 1;

    std::wcout << L"\nВыберите критерии поиска:\n";

    for (int i = 0; i < 6; i++)
        if (!fields_done[i])
        {
            display_get_bill_line(first, fields_name[i]);
            displayed++;
        }
        else
        {
            fields_num[i] = 0;

            for (int j = i + 1; j < 6; j++)
                fields_num[j]--;
        }

    if (choice)
    {
        display_get_bill_line(first, L"начать поиск\n");
        displayed++;
    }

    return displayed;
}

Bill Menu::get_bill(bool fields_done[6])
{
    for (int i = 0; i < 6; i++)
        fields_done[i] = false;

    int fields_num[6], choice = 0;
    Bill bill;

    while (true)
    {
        choice = choose_operation(display_get_bill(fields_done, fields_num, choice));

        int field = 0;
        while (field < 6 && fields_num[field] != choice)
            field++;

        if (field == 6)
            return bill;

        std::wcout << L"\nВведите значение: ";
        get_field(bill, field);
        fields_done[field] = true;
    }
}

int Menu::choose_search_method(TemplateContainer<Bill> &cont)
{
    bool criteria[6];
    Bill template_bill = get_bill(criteria);
    std::wcout << L"\nКакой поиск использовать?\n> линейный\n  бинарный\n";
    return choose_operation(2) == 1
        ? cont.find_linear([template_bill, criteria](Bill bill) { return bill.selective_check(template_bill, criteria); })
        : cont.find_binary([template_bill, criteria](Bill bill) { return bill.selective_check(template_bill, criteria); });
}

void Menu::add_bill(TemplateContainer<Bill> &cont)
{
    Bill new_element;

    std::wcout << L"\nВведите название фирмы: ";
    get_field(new_element, 0);

    std::wcout << L"Введите вид работ: ";
    get_field(new_element, 1);

    std::wcout << L"Введите единицу измерения: ";
    get_field(new_element, 2);

    std::wcout << L"Введите стоимость единицы выполненной работы: ";
    get_field(new_element, 3);

    std::wcout << L"Введите дату исполнения: ";
    get_field(new_element, 4);

    std::wcout << L"Введите объем работ: ";
    get_field(new_element, 5);

    cont.add(new_element);
    std::wcout << L"\n";
}

void Menu::edit_bill(TemplateContainer<Bill> &cont)
{
    std::wcout << L"\n1. Найдем изменяемый счет.";
    int index = choose_search_method(cont);
    std::wcout << L"\n";

    if (index == -1)
    {
        std::wcout << L"Такого счета нет.\n\n";
        return;
    }

    std::wcout << L"Подходящий счет найден.\n";

    while (true)
    {
        std::wcout << L"\n2. Выберите, что нужно изменить:\n> название фирмы\n  вид работ\n  единицу измерения\n  стоимость единицы выполненной работы\n  дату исполнения\n  объем работ\n  завершить\n";
        int choice = choose_operation(7);

        if (choice == 7)
        {
            std::wcout << L"\n";
            return;
        }

        std::wcout << L"\nВведите значение: ";
        get_field(cont[index], choice - 1);
    }
}

void Menu::remove_bill(TemplateContainer<Bill> &cont)
{
    int index = choose_search_method(cont);
    std::wcout << L"\n";

    if (index == -1)
        std::wcout << L"Такого счета нет.";
    else
    {
        cont.remove(index);
        std::wcout << L"Подходящий счет найден и удален.";
    }

    std::wcout << L"\n\n";
}

void Menu::change_estimate_menu(TemplateContainer<Bill> &cont)
{
    int choices = 2;
    std::wcout << L"\n> добавить счет\n";

    if (!cont.empty())
    {
        choices += 2;
        std::wcout << L"  изменить счет\n  удалить счет\n";
    }

    std::wcout << L"  назад\n";

    switch (choose_operation(choices))
    {
        case 1:
            add_bill(cont);
            break;

        case 2:
            if (cont.empty())
            {
                std::wcout << L"\n";
                return;
            }

            edit_bill(cont);
            break;

        case 3:
            remove_bill(cont);
            break;

        case 4:
            std::wcout << L"\n";
    }
}

std::wfstream Menu::get_file(std::ios::openmode mode)
{
    std::wcout << L"\nВведите название файла: ";
    std::wstring filename;
    std::getline(std::wcin, filename);
    std::wfstream file(filename, mode);

    if (!file)
        std::wcout << L"Файл не удалось открыть.\n\n";

    return file;
}

void Menu::get_estimate_from_file(TemplateContainer<Bill> &cont)
{
    std::wfstream file = get_file(std::ios_base::in);

    if (!file)
        return;

    Bill bill;

    while (file >> bill)
        cont.add(bill);

    if (!file.eof())
        std::wcout << L"Часть информации не была считана из файла.\n";

    std::wcout << L"\n";
    file.close();
}

void Menu::print_estimate(TemplateContainer<Bill> &cont)
{
    std::wcout << L"\nКуда вывести?\n> в файл\n  на экран\n";
    std::wfstream file;
    int choice = choose_operation(2);

    if (choice == 2)
    {
        std::wcout << "\n" << cont << "\n";
        return;
    }

    file = get_file(std::ios_base::out | std::ios_base::trunc);

    if (!file)
        return;

    file << cont;
    file.close();
    std::wcout << "\n";
}

void Menu::find_one(TemplateContainer<Bill> &cont)
{
    int index = choose_search_method(cont);
    std::wcout << L"\n";

    if (index == -1)
        std::wcout << L"Такого счета нет.";
    else
        std::wcout << L"Подходящий счет найден.\n" << cont[index];

    std::wcout << L"\n\n";
}

void Menu::find_few(TemplateContainer<Bill> &cont)
{
    bool criteria[6];
    Bill template_bill = get_bill(criteria);
    TemplateContainer<Bill> found = cont.find_linear_all([template_bill, criteria](Bill bill) { return bill.selective_check(template_bill, criteria); });
    std::wcout << "\n";

    if (found.empty())
        std::wcout << L"Таких счетов нет.\n";
    else
        std::wcout << L"Подходящие счета найдены.\n" << found;

    std::wcout << L"\n";
}

int Menu::display_main_menu(bool container_empty)
{
    std::wcout << L"---Главное меню---\n";

    int choices = 3;
    std::wcout << L"> изменить смету\n  загрузить смету из файла\n";

    if (!container_empty)
    {
        choices += 3;
        std::wcout << L"  вывести смету\n  найти один счет по критериям\n  выбрать все счета по критериям\n";
    }

    std::wcout << L"  выйти\n";
    return choices;
}

bool Menu::main_menu(TemplateContainer<Bill> &cont)
{
    switch (choose_operation(display_main_menu(cont.empty())))
    {
        case 1:
            change_estimate_menu(cont);
            break;

        case 2:
            get_estimate_from_file(cont);
            break;

        case 3:
            if (cont.empty())
                return false;

            print_estimate(cont);
            break;

        case 4:
            find_one(cont);
            break;

        case 5:
            find_few(cont);
            break;

        case 6:
            return false;
    }

    return true;
}