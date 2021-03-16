/* Амелина Полина, 2 курс, 91 группа, 1 лаба

Написать программу с текстовым меню, выполняющую следующие функции:

1. Функцию заполнения текстового файла N целыми случайными числами в диапазоне от -M до M.
   Имя файла, N и M поступают на вход функции, файл возвращается функцией как результат (использовать потоки ввода-вывода).
   Реализовать два варианта генерации чисел — в цикле и с использованием алгоритма std::generate.
2. Функцию, получающую файл на вход, и возвращающую последовательный контейнер, заполненный числами из файла.
3. Функцию modify, выполняющую преобразование полученного контейнера.
   На вход функции должен поступать контейнер, в качестве результата функция должна возвращать модифицированный контейнер.
4. Перегруженную функцию modify, принимающую на вход итераторы начала и конца обрабатываемой части контейнера.
5. Функцию, реализующую требуемое преобразование с использованием алгоритма std::transform вместо функции modify.
6. Функцию, реализующую требуемое преобразование с использованием алгоритма std::for_each вместо функции modify.
7. Функции, вычисляющие сумму и среднее арифметическое чисел, содержащихся в контейнере.
8. Функцию вывода результата в файл и на экран (опционально).

№c20. Контейнер: список. Преобразование: заменить каждое третье положительное число средним арифметическим всех чисел. */

#pragma once

#include <fstream>
#include <list>
#include <string>

/* вспомогательные функции, которых нет в условии */

std::wstring input_filename();
std::wfstream open_file(const std::wstring &, std::ios::openmode = std::ios::in | std::ios::out | std::ios::trunc);
bool is_number(const std::wstring &);
int input_number();
int input_positive_number();
int input_number_in_range(int, int);
int generate_random_number(int);
double choose_modified_number(int, double, int &);

/* функции, требуемые условием задачи */

std::wfstream fill_file_with_numbers(const std::wstring &, int, int, bool);
std::list<int> get_numbers_from_file(std::wfstream &);
std::list<double> modify(std::list<int> &);
std::list<double> modify(std::list<int> &, std::list<int>::iterator, std::list<int>::iterator);
std::list<double> modify_transform(std::list<int> &);
std::list<double> modify_transform(std::list<int> &, std::list<int>::iterator, std::list<int>::iterator);
std::list<double> modify_for_each(std::list<int> &);
std::list<double> modify_for_each(std::list<int> &, std::list<int>::iterator, std::list<int>::iterator);
int numbers_sum(const std::list<int> &);
double numbers_average(const std::list<int> &);
template<class T>
void print(std::wostream &, const std::list<T> &);

/* функции, отвечающие за интерфейс. не обрабатывают информацию */

void display_fill_file_with_numbers(std::wfstream &);
bool display_choose_file();
void display_get_numbers_from_file(std::wfstream &, std::list<int> &);
std::list<double> display_modify_range(std::list<int> &, std::list<double> *(std::list<int> &, std::list<int>::iterator, std::list<int>::iterator));
std::list<double> display_modify(std::list<int> &);
void display_print_to_file(const std::list<double> &);
bool display_original_list_menu(std::list<int> &, std::list<double> &, const std::wstring &);
bool display_modified_list_menu(const std::list<double> &);
int display_main_menu(const std::list<int> &, const std::list<double> &, std::wstring &);
bool main_menu(std::wfstream &file, std::list<int> &, std::list<double> &);