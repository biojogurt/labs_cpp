#include "Date.h"

Date::Date(int d, int m, int y)
    : day(d), month(m), year(y)
{
    if (!check_day(d, m, y) || !check_month(m))
        throw std::invalid_argument("Incorrect date");
}

Date::Date(std::wstring date)
    : Date(std::stoi(date.substr(0, date.find(L'.'))),
           std::stoi(date.substr(date.find(L'.') + 1, date.rfind(L'.') - date.find(L'.') + 1)),
           std::stoi(date.substr(date.rfind(L'.') + 1)))
{}

bool Date::check_day(int d, int m, int y)
{
    return d >= 1 && d <= 31 && (m == 2 ? d <= ((y % 4 == 0 && y % 100 != 0 || y % 400 == 0) ? 29 : 28) : (m != 4 && m != 6 && m != 9 && m != 11 || d <= 30));
}

bool Date::check_month(int m)
{
    return m >= 1 && m <= 12;
}

bool Date::operator<(const Date &other) const
{
    return year < other.year
        || month < other.month
        || day < other.day;
}

bool Date::operator>(const Date &other) const
{
    return year > other.year
        || month > other.month
        || day > other.day;
}

bool Date::operator==(const Date &other) const
{
    return year == other.year
        && month == other.month
        && day == other.day;
}

bool Date::operator!=(const Date &other) const
{
    return !(*this == other);
}

std::wistream &operator>>(std::wistream &is, Date &date)
{
    std::wstring date_string;
    std::getline(is, date_string);

    try
    {
        date = Date(date_string);
    }
    catch (...)
    {
        is.setstate(std::ios_base::failbit);
        return is;
    }

    return is;
}

std::wostream &operator<<(std::wostream &os, const Date &date)
{
    os << date.day << L"." << date.month << L"." << date.year;
    return os;
}