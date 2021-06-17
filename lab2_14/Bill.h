#pragma once
#include "Date.h"

class Bill
{
public:
    std::wstring firm_name;
    std::wstring work_type;
    std::wstring unit;
    int work_cost;
    Date completion_date;
    int work_amount;

    Bill(std::wstring = L"", std::wstring = L"", std::wstring = L"", int = -1, Date = Date(), int = -1);

private:
    template<class T>
    int selective_check_one(const T &, const T &) const;

public:
    int selective_check(const Bill &, const bool[6]) const;

    bool operator<(const Bill &) const;
    bool operator>(const Bill &) const;

    friend std::wistream &operator>>(std::wistream &, Bill &);
    friend std::wostream &operator<<(std::wostream &, const Bill &);
};