#pragma once
#include "Bill.h"
#include "Container.h"

class Menu
{
private:
    static int choose_operation(int);

    static bool is_number(const std::wstring &);
    static int get_number();
    static Date get_date();

    static void get_field(Bill &, int);
    static void display_get_bill_line(bool &, const std::wstring &);
    static int display_get_bill(bool[6], int[6], int);
    static Bill get_bill(bool[6]);

    static bool choose_search_method(const TemplateContainer<Bill> &, Bill &);
    static TemplateContainer<Bill> choose_search_method_all(const TemplateContainer<Bill> &);
    static int index_linear_search(const TemplateContainer<Bill> &);

    static void add_bill(TemplateContainer<Bill> &);
    static void edit_bill(TemplateContainer<Bill> &);
    static void remove_bill(TemplateContainer<Bill> &);
    static void change_estimate_menu(TemplateContainer<Bill> &);

    static std::wfstream get_file(std::ios::openmode);
    static void get_estimate_from_file(TemplateContainer<Bill> &);
    static void print_estimate(const TemplateContainer<Bill> &);

    static void find_one(const TemplateContainer<Bill> &);
    static void find_all(const TemplateContainer<Bill> &);

    static int display_main_menu(bool);

public:
    static bool main_menu(TemplateContainer<Bill> &);
};