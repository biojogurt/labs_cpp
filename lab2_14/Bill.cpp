#include "Bill.h"

Bill::Bill(std::wstring fn, std::wstring wt, std::wstring u, int wc, Date cd, int wa)
    : firm_name(fn), work_type(wt), unit(u), work_cost(wc), completion_date(cd), work_amount(wa)
{}

template<class T>
int Bill::selective_check_one(const T &elem1, const T &elem2) const
{
    if (elem1 < elem2)
        return -1;

    if (elem1 > elem2)
        return 1;

    return 0;
}

int Bill::selective_check(const Bill &other, const bool criteria[6]) const
{
    if (criteria[0])
    {
        int result = selective_check_one(firm_name, other.firm_name);
        if (result != 0)
            return result;
    }

    if (criteria[1])
    {
        int result = selective_check_one(work_type, other.work_type);
        if (result != 0)
            return result;
    }

    if (criteria[2])
    {
        int result = selective_check_one(unit, other.unit);
        if (result != 0)
            return result;
    }

    if (criteria[3])
    {
        int result = selective_check_one(work_cost, other.work_cost);
        if (result != 0)
            return result;
    }

    if (criteria[4])
    {
        int result = selective_check_one(completion_date, other.completion_date);
        if (result != 0)
            return result;
    }

    return criteria[5] ? selective_check_one(work_amount, other.work_amount) : 0;
}

bool Bill::operator<(const Bill &other) const
{
    return firm_name < other.firm_name
        || work_type < other.work_type
        || unit < other.unit
        || work_cost < other.work_cost
        || completion_date < other.completion_date
        || work_amount < other.work_amount;
}

bool Bill::operator>(const Bill &other) const
{
    return firm_name > other.firm_name
        || work_type > other.work_type
        || unit > other.unit
        || work_cost > other.work_cost
        || completion_date > other.completion_date
        || work_amount > other.work_amount;
}

std::wistream &operator>>(std::wistream &is, Bill &bill)
{
    std::wstring firm_name, work_type, unit, work_cost, completion_date, work_amount;

    std::getline(is, firm_name);
    std::getline(is, work_type);
    std::getline(is, unit);
    std::getline(is, work_cost);
    std::getline(is, completion_date);
    std::getline(is, work_amount);

    try
    {
        bill = Bill(firm_name, work_type, unit, std::stoi(work_cost), Date(completion_date), std::stoi(work_amount));
    }
    catch (...)
    {
        is.setstate(std::ios_base::failbit);
        return is;
    }

    return is;
}

std::wostream &operator<<(std::wostream &os, const Bill &bill)
{
    os << bill.firm_name << "\n"
        << bill.work_type << "\n"
        << bill.unit << "\n"
        << bill.work_cost << "\n"
        << bill.completion_date << "\n"
        << bill.work_amount;

    return os;
}