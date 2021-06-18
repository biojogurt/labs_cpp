#include "Date.h"

Date::Date(int d, int m, int y)
    : day(d), month(m), year(y)
{
    if (!check_day(day, month, year) || !check_month(month))
        throw std::invalid_argument("Incorrect date");
}

Date::Date(std::wstring date)
{
    size_t first_dot = date.find(L"."), last_dot = date.rfind(L".");

    if (first_dot == std::wstring::npos || last_dot == std::wstring::npos || first_dot == last_dot)
        throw std::invalid_argument("Incorrect date");

    try
    {
        day = std::stoi(date.substr(0, first_dot));
        month = std::stoi(date.substr(first_dot + 1, last_dot - first_dot + 1));
        year = std::stoi(date.substr(last_dot + 1));
    }
    catch (...)
    {
        throw std::invalid_argument("Incorrect date");
    }

    if (!check_day(day, month, year) || !check_month(month))
        throw std::invalid_argument("Incorrect date");
}

bool Date::check_day(int d, int m, int y)
{
    return d >= 1 && d <= 31 && (m == 2 ? d <= ((y % 4 == 0 && y % 100 != 0 || y % 400 == 0) ? 29 : 28) : (m != 4 && m != 6 && m != 9 && m != 11 || d <= 30));
}

bool Date::check_month(int m)
{
    return m >= 1 && m <= 12;
}

Date &Date::operator=(const Date &other)
{
    if (this == &other)
        return *this;

    day = other.day;
    month = other.month;
    year = other.year;
    return *this;
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