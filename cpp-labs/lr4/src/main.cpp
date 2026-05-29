#include "../include/simulator.h"
#include "../include/herbivore.h"
#include "../include/predator.h"
#include "../include/omnivore.h"
#include <iostream>
#include <limits>
#include <memory>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getIntInput(const std::string& prompt, int min = 0, int max = 1000000) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) {
            clearInput();
            return value;
        }
        std::cout << "Некорректный ввод. Попробуйте снова.\n";
        clearInput();
    }
}

void showMainMenu() {
    std::cout << "\n=== ЭВОЛЮЦИОННЫЙ СИМУЛЯТОР ===\n";
    std::cout << "1. Начать новую симуляцию\n";
    std::cout << "2. Загрузить сохранение\n";
    std::cout << "3. Выход\n";
    std::cout << "Выберите действие: ";
}

void showSimulationMenu() {
    std::cout << "\n=== УПРАВЛЕНИЕ ===\n";
    std::cout << "1. Следующий день\n";
    std::cout << "2. Следующий месяц (30 дней)\n";
    std::cout << "3. Автоматический режим (вкл/выкл)\n";
    std::cout << "4. Ускорить время\n";
    std::cout << "5. Замедлить время\n";
    std::cout << "6. Показать статистику\n";
    std::cout << "7. Показать карту\n";
    std::cout << "8. Информация о виде\n";
    std::cout << "9. Вмешательство\n";
    std::cout << "10. Сохранить\n";
    std::cout << "11. Выйти в главное меню\n";
    std::cout << "Выберите действие: ";
}

void showInterventionMenu() {
    std::cout << "\n=== ВМЕШАТЕЛЬСТВО ===\n";
    std::cout << "1. Добавить существо\n";
    std::cout << "2. Удалить существо\n";
    std::cout << "3. Изменить местность\n";
    std::cout << "4. Изменить растения\n";
    std::cout << "5. Назад\n";
    std::cout << "Выберите действие: ";
}

void handleIntervention(Simulator& sim) {
    while (true) {
        showInterventionMenu();
        int choice = getIntInput("", 1, 5);

        switch (choice) {
            case 1: {
                std::cout << "\nВыберите тип: 1-Травоядное, 2-Хищник, 3-Всеядное: ";
                int type = getIntInput("", 1, 3);
                int x = getIntInput("X координата: ", 0, 100);
                int y = getIntInput("Y координата: ", 0, 100);

                std::string typeStr;
                if (type == 1) typeStr = "herbivore";
                else if (type == 2) typeStr = "predator";
                else typeStr = "omnivore";

                sim.addCreature(typeStr, x, y);
                break;
            }
            case 2: {
                int id = getIntInput("Введите ID существа: ", 1);
                sim.removeCreature(id);
                break;
            }
            case 3: {
                int x = getIntInput("X координата: ", 0, 100);
                int y = getIntInput("Y координата: ", 0, 100);
                std::cout << "Тип: 0-Лес, 1-Поле, 2-Пустыня, 3-Вода, 4-Горы: ";
                int terrain = getIntInput("", 0, 4);
                sim.changeTerrain(x, y, terrain);
                break;
            }
            case 4: {
                int x = getIntInput("X координата: ", 0, 100);
                int y = getIntInput("Y координата: ", 0, 100);
                int amount = getIntInput("Изменение количества растений (-100 до 100): ", -100, 100);
                sim.modifyPlants(x, y, amount);
                break;
            }
            case 5:
                return;
        }
    }
}

void runSimulation(Simulator& sim) {
    while (sim.isRunning()) {
        showSimulationMenu();
        int choice = getIntInput("", 1, 11);

        switch (choice) {
            case 1:
                sim.step();
                break;

            case 2:
                sim.stepMonth();
                break;

            case 3: {
                static bool autoMode = false;
                autoMode = !autoMode;
                sim.setAutoMode(autoMode);
                std::cout << "Автоматический режим " 
                          << (autoMode ? "включен" : "выключен") << "\n";
                if (autoMode) {
                    sim.run();
                }
                break;
            }

            case 4:
                sim.increaseSpeed();
                std::cout << "Скорость увеличена\n";
                break;

            case 5:
                sim.decreaseSpeed();
                std::cout << "Скорость уменьшена\n";
                break;

            case 6:
                sim.displayStats();
                break;

            case 7:
                sim.displayMap();
                break;

            case 8: {
                std::cout << "\nДоступные виды:\n";
                std::cout << "1. Травоядное\n";
                std::cout << "2. Хищник\n";
                std::cout << "3. Всеядное\n";
                int type = getIntInput("Выберите вид: ", 1, 3);

                std::string species;
                if (type == 1) species = "Травоядное";
                else if (type == 2) species = "Хищник";
                else species = "Всеядное";

                sim.displaySpeciesInfo(species);
                break;
            }

            case 9:
                handleIntervention(sim);
                break;

            case 10: {
                std::string filename;
                std::cout << "Имя файла: ";
                clearInput();
                std::getline(std::cin, filename);
                if (filename.empty()) filename = "save.txt";
                sim.save(filename);
                break;
            }

            case 11:
                return;
        }
    }

    std::cout << "\nСимуляция завершена.\n";
    std::cout << "1. Начать новую симуляцию\n";
    std::cout << "2. Загрузить сохранение\n";
    std::cout << "3. Выйти\n";
    getIntInput("Выберите действие: ", 1, 3);
}

int main() {
    std::cout << "==============================================\n";
    std::cout << "  ЭВОЛЮЦИОННЫЙ СИМУЛЯТОР \"ВЫЖИВАНИЕ ВИДОВ\"\n";
    std::cout << "  Лабораторная работа №4\n";
    std::cout << "==============================================\n";

    while (true) {
        showMainMenu();
        int choice = getIntInput("", 1, 3);

        switch (choice) {
            case 1: {
                int mapSize = getIntInput("\nРазмер карты (10-50): ", 10, 50);

                std::unique_ptr<Simulator> sim = std::make_unique<Simulator>(mapSize);

                int herbivores = getIntInput("Количество травоядных (1-50): ", 1, 50);
                int predators = getIntInput("Количество хищников (1-30): ", 1, 30);
                int omnivores = getIntInput("Количество всеядных (1-40): ", 1, 40);

                sim->initialize(herbivores, predators, omnivores);

                std::cout << "\n=== ДЕМОНСТРАЦИЯ КОНСТРУКТОРОВ/ДЕСТРУКТОРОВ ===\n";
                std::cout << "Всего создано существ: " << Creature::getTotalCreated() << "\n";

                runSimulation(*sim);
                break;
            }

            case 2: {
                std::string filename;
                std::cout << "Имя файла: ";
                clearInput();
                std::getline(std::cin, filename);
                if (filename.empty()) filename = "save.txt";

                auto sim = std::make_unique<Simulator>(20);
                sim->load(filename);

                if (sim->isRunning()) {
                    runSimulation(*sim);
                }
                break;
            }

            case 3:
                std::cout << "\nВыход из программы. До свидания!\n";
                return 0;
        }
    }

    return 0;
}
