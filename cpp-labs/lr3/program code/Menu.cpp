#include "Menu.h"
#include <iostream>
#include <limits>
#include <stdexcept>

int Menu::getChoice(int max) {
    int choice;
    std::cout << "\nВыберите задачу: ";
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::runtime_error("Некорректный ввод. Ожидается число.");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (choice < 0 || choice > max) {
        throw std::out_of_range("Выбран несуществующий пункт меню.");
    }
    return choice;
}

void Menu::addTask(const std::string& desc, std::function<void()> action) {
    items.push_back({desc, action});
}

void Menu::showSubMenu(int taskIndex) {
    std::cout << "\nЗадача завершена.\n";
    std::cout << "1. Выполнить задачу еще раз\n";
    std::cout << "2. Вернуться в главное меню\n";
    try {
        int choice = getChoice(2);
        if (choice == 1) {
            size_t idx = static_cast<size_t>(taskIndex);
            std::cout << "\n--- " << items[idx].description << " ---\n";
            items[idx].action();
            showSubMenu(taskIndex);
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}

void Menu::run() {
    bool running = true;
    while (running) {
        std::cout << "\n=== ЛАБОРАТОРНАЯ РАБОТА 3 ===\n";
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << "Задача " << (i + 1) << ". " << items[i].description << "\n";
        }
        std::cout << "0. Выход\n";

        try {
            int choice = getChoice(static_cast<int>(items.size()));
            if (choice == 0) {
                running = false;
                std::cout << "Программа завершена.\n";
            } else {
                size_t idx = static_cast<size_t>(choice - 1);
                std::cout << "\n--- " << items[idx].description << " ---\n";
                items[idx].action();
                showSubMenu(choice - 1);
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
    }
}