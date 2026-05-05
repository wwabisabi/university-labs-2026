#pragma once
#include <vector>
#include <string>
#include <functional>
#include <iostream>

class Menu {
private:
    struct MenuItem {
        std::string description;
        std::function<void()> action;
    };
    std::vector<MenuItem> items;

    int getChoice(int max);
    void showSubMenu(int taskIndex);

public:
    void addTask(const std::string& desc, std::function<void()> action);
    void run();
};