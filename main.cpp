#include <iostream>
#include <algorithm>
#include <locale>
#include "include/list.h"
#include "include/my_pmr.h"
#include "include/square.h"

template<typename U>
auto print_element(U& el) -> decltype(el.print(std::cout), void()) {
    el.print(std::cout);
    std::cout << std::endl;
}

inline void print_element(int& el) {
    std::cout << el << std::endl;
}

template<typename U>
auto read_element(U& el) -> decltype(el.read(std::cin), void()) {
    el.read(std::cin);
}

inline void read_element(int& el) {
    std::cin >> el;
}

void Menu1() {
    std::cout << "1. Добавить элемент в список\n";
    std::cout << "2. Удалить элемент\n";
    std::cout << "3. Вывести элемент\n";
    std::cout << "5. Подсчитать элементы по условию\n";
}

void PushMenu() {
    std::cout << "1. Добавить элемент в начало списка\n";
    std::cout << "2. Добавить элемент в конец списка\n";
}

void DeleteMenu() {
    std::cout << "1. Удалить элемент в начале списка\n";
    std::cout << "2. Удалить элемент в конце списка\n";
}

void PrintMenu() {
    std::cout << "1. Вывести первый элемент\n";
    std::cout << "2. Вывести последний элемент\n";
}

template<typename T>
int RunListApp() {
    setlocale(LC_ALL, "");

    LinearMemSource my_resource(500);
    D_linked_list<T> MyList(&my_resource);    T TempElement;

    while (true) {
        Menu1();
        int n, m;
        double cond; 
        std::cin >> n;

        switch (n) {
        case 1: 
            read_element(TempElement);
            PushMenu();
            std::cin >> m;
            switch (m) {
            case 1: 
                MyList.push_front(TempElement);
                break;
            case 2:
                MyList.push_back(TempElement);
                break;
            default:
                break;
            }
            break;

        case 2:
            DeleteMenu();
            std::cin >> m;
            switch (m) {
            case 1:
                MyList.pop_front();
                break;
            case 2:
                MyList.pop_back();
                break;
            default:
                break;
            }
            break;

        case 4:
            std::for_each(MyList.begin(), MyList.end(),
                [](T& X) { print_element(X); });
            break;

        case 5: 
            std::cin >> cond;
            std::cout << std::count_if(MyList.begin(), MyList.end(),
                [=](T& X) { 
                    if constexpr (std::is_arithmetic_v<T>)
                        return X > cond; 
                    else
                        return X.square() > cond; 
                }) << std::endl;
            break;

        default:
            return 0;
        }
    }

    return 0;
}

int main() {
    return RunListApp<int>();
    // return RunListApp<Square<int>>();
}
