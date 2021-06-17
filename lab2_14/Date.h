#pragma once
#include <iostream>
#include <string>

class Date
{
private:
    int day;
    int month;
    int year;

public:
    Date(int = 1, int = 1, int = -1);
    Date(std::wstring);

    static bool check_day(int, int, int);
    static bool check_month(int);

    bool operator<(const Date &) const;
    bool operator>(const Date &) const;
    bool operator==(const Date &) const;
    bool operator!=(const Date &) const;

    friend std::wistream &operator>>(std::wistream &, Date &);
    friend std::wostream &operator<<(std::wostream &, const Date &);
};