#pragma once
#include <functional>
#include <iostream>
#include <vector>

template <class T>
class TemplateContainer
{
private:
    std::vector<T> container;

public:
    typedef std::function<int(const T &)> check;

    size_t size() const;

    void add(const T &);
    bool remove(int);

    int find_linear(check) const;
    bool find_linear(check, T &) const;
    TemplateContainer<T> find_linear_all(check) const;

private:
    void sort();

public:
    bool find_binary(check, T &) const;
    TemplateContainer<T> find_binary_all(check) const;

    TemplateContainer<T> &operator=(const TemplateContainer<T> &);
    T &operator[](size_t i);
    const T &operator[](size_t i) const;

    template <class T>
    friend std::wistream &operator>>(std::wistream &, TemplateContainer<T> &);
    template <class T>
    friend std::wostream &operator<<(std::wostream &, const TemplateContainer<T> &);
};

template<class T>
size_t TemplateContainer<T>::size() const
{
    return container.size();
}

template <class T>
void TemplateContainer<T>::add(const T &element)
{
    container.emplace_back(element);
}

template <class T>
bool TemplateContainer<T>::remove(int pos)
{
    if (pos >= container.size())
        return false;

    container.erase(container.begin() + pos);
    return true;
}

template <class T>
int TemplateContainer<T>::find_linear(check checker) const
{
    for (int i = 0; i < container.size(); i++)
        if (checker(container[i]) == 0)
            return i;

    return -1;
}

template <class T>
bool TemplateContainer<T>::find_linear(check checker, T &found) const
{
    for (int i = 0; i < container.size(); i++)
        if (checker(container[i]) == 0)
        {
            found = container[i];
            return true;
        }

    return false;
}

template<class T>
TemplateContainer<T> TemplateContainer<T>::find_linear_all(check checker) const
{
    TemplateContainer<T> found;

    for (int i = 0; i < container.size(); i++)
        if (checker(container[i]) == 0)
            found.add(container[i]);

    return found;
}

template<class T>
void TemplateContainer<T>::sort()
{
    for (int i = 1; i < container.size(); i++)
    {
        T key = container[i];
        int low = 0, high = static_cast<int>(container.size()), mid = (low + high) / 2;

        while (low != high)
        {
            if (container[mid] < key)
                low = mid + 1;
            else
                high = mid;

            mid = (low + high) / 2;
        }

        for (int j = i; j > low; j--)
            container[j] = container[j - 1];

        container[low] = key;
    }
}

template <class T>
bool TemplateContainer<T>::find_binary(check checker, T &found) const
{
    TemplateContainer<T> sorted = *this;
    sorted.sort();

    int low = 0, high = static_cast<int>(sorted.size()), mid = (low + high) / 2;

    while (mid != high && checker(sorted[mid]) != 0)
    {
        if (checker(sorted[mid]) < 0)
            low = mid + 1;
        else
            high = mid;

        mid = (low + high) / 2;
    }

    if (mid == high)
        return false;
    else
    {
        found = sorted[mid];
        return true;
    }
}

template<class T>
TemplateContainer<T> TemplateContainer<T>::find_binary_all(check checker) const
{
    TemplateContainer<T> sorted = *this;
    sorted.sort();

    int low = 0, high = static_cast<int>(sorted.size()), mid = (low + high) / 2;

    while (low != high)
    {
        if (checker(sorted[mid]) < 0)
            low = mid + 1;
        else
            high = mid;

        mid = (low + high) / 2;
    }

    int lower_bound = mid;
    low = 0, high = static_cast<int>(sorted.size()), mid = (low + high) / 2;

    while (low != high)
    {
        if (checker(sorted[mid]) <= 0)
            low = mid + 1;
        else
            high = mid;

        mid = (low + high) / 2;
    }

    int upper_bound = mid;
    TemplateContainer<T> found;

    for (int i = lower_bound; i < upper_bound; i++)
        found.add(sorted[i]);

    return found;
}

template<class T>
TemplateContainer<T> &TemplateContainer<T>::operator=(const TemplateContainer<T> &other)
{
    if (this == &other)
        return *this;

    container = std::vector<T>(other.size());

    for (int i = 0; i < other.size(); i++)
        container[i] = other[i];

    return *this;
}

template<class T>
T &TemplateContainer<T>::operator[](std::size_t i)
{
    return container[i];
}

template<class T>
const T &TemplateContainer<T>::operator[](size_t i) const
{
    return container[i];
}

template <class T>
std::wistream &operator>>(std::wistream &is, TemplateContainer<T> &cont)
{
    T element;

    if (is >> element)
        cont.add(element);

    return is;
}

template <class T>
std::wostream &operator<<(std::wostream &os, const TemplateContainer<T> &cont)
{
    for (const T &element : cont.container)
        os << element << "\n";

    return os;
}