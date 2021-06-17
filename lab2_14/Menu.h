#pragma once
#include "Bill.h"
#include "Container.h"

class Menu
{
private:
    static int choose_operation(int);

    static bool is_number(std::wstring);
    static int get_number();
    static Date get_date();

    static void get_field(Bill &, int);
    static void display_get_bill_line(bool &, std::wstring);
    static int display_get_bill(bool[6], int[6], int);
    static Bill get_bill(bool[6]);

    static int choose_search_method(TemplateContainer<Bill> &);

    static void add_bill(TemplateContainer<Bill> &);
    static void edit_bill(TemplateContainer<Bill> &);
    static void remove_bill(TemplateContainer<Bill> &);
    static void change_estimate_menu(TemplateContainer<Bill> &);

    static std::wfstream get_file(std::ios::openmode);
    static void get_estimate_from_file(TemplateContainer<Bill> &);
    static void print_estimate(TemplateContainer<Bill> &);

    static void find_one(TemplateContainer<Bill> &);
    static void find_few(TemplateContainer<Bill> &);

    static int display_main_menu(bool);

public:
    static bool main_menu(TemplateContainer<Bill> &);
};