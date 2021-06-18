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
                        std::wcout << L"\x1b[" + std::to_wstring(choices_amount + 1 - position) + L"B\x1b[?25h\n";
                        return position;
                    }
            }
        }
    }
}

bool Menu::is_number(const std::wstring &number)
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

void Menu::display_get_bill_line(bool &first, const std::wstring &field_name)
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

    std::wcout << L"Выберите критерии поиска:\n";

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

        std::wcout << L"Введите значение: ";
        get_field(bill, field);
        std::wcout << L"\n";
        fields_done[field] = true;
    }
}

bool Menu::choose_search_method(const TemplateContainer<Bill> &cont, Bill &bill)
{
    bool criteria[6];
    Bill template_bill = get_bill(criteria);
    std::wcout << L"Какой поиск использовать?\n> линейный\n  бинарный\n";
    return choose_operation(2) == 1
        ? cont.find_linear([template_bill, criteria](Bill bill) { return bill.selective_check(template_bill, criteria); }, bill)
        : cont.find_binary([template_bill, criteria](Bill bill) { return bill.selective_check(template_bill, criteria); }, bill);
}

TemplateContainer<Bill> Menu::choose_search_method_all(const TemplateContainer<Bill> &cont)
{
    bool criteria[6];
    Bill template_bill = get_bill(criteria);
    std::wcout << L"Какой поиск использовать?\n> линейный\n  бинарный\n";
    return choose_operation(2) == 1
        ? cont.find_linear_all([template_bill, criteria](Bill bill) { return bill.selective_check(template_bill, criteria); })
        : cont.find_binary_all([template_bill, criteria](Bill bill) { return bill.selective_check(template_bill, criteria); });
}

int Menu::index_linear_search(const TemplateContainer<Bill> &cont)
{
    bool criteria[6];
    Bill template_bill = get_bill(criteria);
    return cont.find_linear([template_bill, criteria](Bill bill) { return bill.selective_check(template_bill, criteria); });
}

void Menu::add_bill(TemplateContainer<Bill> &cont)
{
    Bill new_element;

    std::wcout << L"Введите название фирмы: ";
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
    save_estimate(cont);
    std::wcout << L"\n";
}

void Menu::edit_bill(TemplateContainer<Bill> &cont)
{
    int index = index_linear_search(cont);

    if (index == -1)
    {
        std::wcout << L"Такого счета нет.\n\n";
        return;
    }

    std::wcout << L"Найден следующий счет:\n" << cont[index] << L"\n\nИзменить его?\n> да\n  нет\n";

    if (choose_operation(2) == 2)
        return;

    while (true)
    {
        std::wcout << L"Выберите, что нужно изменить:\n> название фирмы\n  вид работ\n  единицу измерения\n  стоимость единицы выполненной работы\n  дату исполнения\n  объем работ\n  завершить\n";
        int choice = choose_operation(7);

        if (choice == 7)
            return save_estimate(cont);

        std::wcout << L"Введите значение: ";
        get_field(cont[index], choice - 1);
        std::wcout << L"\n";
    }
}

void Menu::remove_bill(TemplateContainer<Bill> &cont)
{
    int index = index_linear_search(cont);

    if (index == -1)
    {
        std::wcout << L"Такого счета нет.\n\n";
        return;
    }

    std::wcout << L"Найден следующий счет:\n" << cont[index] << L"\n\nУдалить его?\n> да\n  нет\n";

    if (choose_operation(2) == 1)
    {
        cont.remove(index);
        save_estimate(cont);
        std::wcout << L"Счет удален.\n\n";
    }
}

void Menu::change_estimate_menu(TemplateContainer<Bill> &cont)
{
    int choices = 2;
    std::wcout << L"> добавить счет\n";

    if (cont.size())
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
            if (!cont.size())
                return;

            edit_bill(cont);
            break;

        case 3:
            remove_bill(cont);
            break;
    }
}

std::wfstream Menu::get_file(std::ios::openmode mode)
{
    std::wcout << L"Введите название файла: ";
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

    while (file >> cont);

    if (!file.eof())
        std::wcout << L"Часть информации не была считана из файла.\n";

    file.close();
    save_estimate(cont);
    std::wcout << L"\n";
}

void Menu::print_estimate(const TemplateContainer<Bill> &cont)
{
    std::wcout << L"Куда вывести?\n> в файл\n  на экран\n";

    if (choose_operation(2) == 2)
    {
        std::wcout << cont << "\n";
        return;
    }

    std::wfstream file = get_file(std::ios_base::out | std::ios_base::trunc);

    if (!file)
        return;

    file << cont;
    file.close();
    std::wcout << "\n";
}

void Menu::find_one(const TemplateContainer<Bill> &cont)
{
    Bill bill;

    if (!choose_search_method(cont, bill))
        std::wcout << L"Такого счета нет.";
    else
        std::wcout << L"Подходящий счет найден.\n" << bill;

    std::wcout << L"\n\n";
}

void Menu::find_all(const TemplateContainer<Bill> &cont)
{
    TemplateContainer<Bill> found = choose_search_method_all(cont);

    if (!found.size())
        std::wcout << L"Таких счетов нет.\n\n";
    else
    {
        std::wcout << L"Подходящие счета найдены.\n\n";
        print_estimate(found);
    }
}

void Menu::save_estimate(const TemplateContainer<Bill> &cont)
{
    std::wfstream file(L"estimate", std::ios_base::out | std::ios_base::trunc);
    DWORD attributes = GetFileAttributes(L"estimate");

    if (!(attributes & FILE_ATTRIBUTE_HIDDEN))
        SetFileAttributes(L"estimate", attributes | FILE_ATTRIBUTE_HIDDEN);

    file << cont;
    file.close();
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

void Menu::main_menu(TemplateContainer<Bill> &cont)
{
    std::wfstream file(L"estimate", std::ios_base::in);
    while (file >> cont);
    file.close();

    while (true)
    {
        switch (choose_operation(display_main_menu(!cont.size())))
        {
            case 1:
                change_estimate_menu(cont);
                break;

            case 2:
                get_estimate_from_file(cont);
                break;

            case 3:
                if (!cont.size())
                    return;

                print_estimate(cont);
                break;

            case 4:
                find_one(cont);
                break;

            case 5:
                find_all(cont);
                break;

            case 6:
                return;
        }
    }
}