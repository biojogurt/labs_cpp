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

    T &operator[](size_t i);

    void add(const T &);
    bool remove(const T &);
    bool remove(int);

    bool empty() const;

private:
    void sort();

public:
    int find_linear(check) const;
    TemplateContainer<T> find_linear_all(check) const;
    int find_binary(check);

    template <class T>
    friend std::wistream &operator>>(std::wistream &, TemplateContainer<T> &);
    template <class T>
    friend std::wostream &operator<<(std::wostream &, const TemplateContainer<T> &);
};

template <class T>
void TemplateContainer<T>::add(const T &element)
{
    container.emplace_back(element);
}

template <class T>
bool TemplateContainer<T>::remove(const T &element)
{
    int pos = find_binary(element);

    if (pos == -1)
        return false;

    container.erase(container.begin() + pos);
    return true;
}

template <class T>
bool TemplateContainer<T>::remove(int pos)
{
    if (pos >= container.size())
        return false;

    container.erase(container.begin() + pos);
    return true;
}


template<class T>
bool TemplateContainer<T>::empty() const
{
    return container.size() == 0;
}

template<class T>
T &TemplateContainer<T>::operator[](std::size_t i)
{
    return container[i];
}

template<class T>
void TemplateContainer<T>::sort()
{
    for (int i = 1; i < container.size(); i++)
    {
        T key = container[i];
        int low = 0,
            high = static_cast<int>(container.size()),
            mid = (low + high) / 2;

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
int TemplateContainer<T>::find_linear(check checker) const
{
    for (int i = 0; i < container.size(); i++)
        if (checker(container[i]) == 0)
            return i;

    return -1;
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

template <class T>
int TemplateContainer<T>::find_binary(check checker)
{
    sort();
    int low = 0,
        high = static_cast<int>(container.size()),
        mid = (low + high) / 2;

    while (mid != high && checker(container[mid]) != 0)
    {
        if (checker(container[mid]) < 0)
            low = mid + 1;
        else
            high = mid;

        mid = (low + high) / 2;
    }

    return mid != high ? mid : -1;
}

template <class T>
std::wistream &operator>>(std::wistream &is, TemplateContainer<T> &cont)
{
    T element{};

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