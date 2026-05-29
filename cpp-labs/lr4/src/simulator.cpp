#include "../include/simulator.h"
#include "../include/herbivore.h"
#include "../include/predator.h"
#include "../include/omnivore.h"
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>

Simulator::Simulator(int mapSize)
    : map(mapSize), running(false), autoMode(false), timeSpeed(1) {}

void Simulator::initialize(int herbivores, int predators, int omnivores) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, map.getSize() - 1);

    for (int i = 0; i < herbivores; ++i) {
        auto creature = std::make_unique<Herbivore>();
        map.addCreature(std::move(creature), dis(gen), dis(gen));
    }

    for (int i = 0; i < predators; ++i) {
        auto creature = std::make_unique<Predator>();
        map.addCreature(std::move(creature), dis(gen), dis(gen));
    }

    for (int i = 0; i < omnivores; ++i) {
        auto creature = std::make_unique<Omnivore>();
        map.addCreature(std::move(creature), dis(gen), dis(gen));
    }

    running = true;
    std::cout << "Симуляция инициализирована: " << herbivores << " травоядных, "
              << predators << " хищников, " << omnivores << " всеядных.";
}

void Simulator::step() {
    updateResources();
    processCreatures();
    updateStates();
    handleReproduction();
    removeDeadCreatures();

    auto creatures = map.getAllCreatures();
    stats.update(creatures);
    stats.nextDay();

    std::cout << "День " << stats.getCurrentDay() << " завершён. Популяция: "
              << creatures.size() << "";

    if (creatures.empty()) {
        std::cout << "Все существа вымерли! Симуляция завершена.";
        running = false;
    }
}

void Simulator::stepMonth() {
    for (int i = 0; i < 30; ++i) {
        step();
        if (!running) break;
        if (autoMode) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / timeSpeed));
        }
    }
}

void Simulator::run() {
    while (running && autoMode) {
        step();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / timeSpeed));
    }
}

void Simulator::updateResources() {
    map.regenerateAllPlants();

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    if (dis(gen) < 0.1) {
        std::cout << "Случайное событие: ";
        if (dis(gen) < 0.5) {
            std::cout << "Засуха! Растения уменьшены.";
        } else {
            std::cout << "Дождь! Растения увеличены.";
        }
    }
}

void Simulator::processCreatures() {
    int worldSize = map.getSize();
    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < worldSize; ++i) {
        for (int j = 0; j < worldSize; ++j) {
            positions.emplace_back(i, j);
        }
    }

    std::shuffle(positions.begin(), positions.end(), std::mt19937{std::random_device{}()});

    for (const auto& [i, j] : positions) {
        Tile* tile = map.getTile(i, j);
        if (!tile) continue;

        std::size_t index = 0;
        while (index < tile->getCreatures().size()) {
            auto& creatures = tile->getCreatures();
            if (index >= creatures.size()) break;

            Creature* creature = creatures[index].get();
            if (!creature || !creature->isAlive()) {
                ++index;
                continue;
            }

            creature->move(map);

            Tile* newTile = map.getTile(creature->getX(), creature->getY());
            if (newTile) {
                creature->eat(map);
            }

            if (newTile == tile) {
                ++index;
            }
        }
    }
}

void Simulator::updateStates() {
    auto creatures = map.getAllCreatures();
    for (auto* creature : creatures) {
        if (creature && creature->isAlive()) {
            creature->increaseAge();
        }
    }
}

void Simulator::handleReproduction() {
    auto creatures = map.getAllCreatures();
    std::vector<std::unique_ptr<Creature>> newborns;

    for (size_t i = 0; i < creatures.size(); ++i) {
        if (!creatures[i] || !creatures[i]->isAlive()) continue;
        for (size_t j = i + 1; j < creatures.size(); ++j) {
            if (!creatures[j] || !creatures[j]->isAlive()) continue;

            if (creatures[i]->getX() == creatures[j]->getX() &&
                creatures[i]->getY() == creatures[j]->getY() &&
                creatures[i]->getSpecies() == creatures[j]->getSpecies()) {

                auto child = creatures[i]->reproduce(*creatures[j]);
                if (child) {
                    stats.recordBirth(child->getSpecies());
                    newborns.push_back(std::move(child));
                }
            }
        }
    }

    for (auto& newborn : newborns) {
        int x = newborn->getX();
        int y = newborn->getY();
        map.addCreature(std::move(newborn), x, y);
    }
}

void Simulator::removeDeadCreatures() {
    for (int i = 0; i < map.getSize(); ++i) {
        for (int j = 0; j < map.getSize(); ++j) {
            auto* tile = map.getTile(i, j);
            if (!tile) continue;

            auto& creatures = tile->getCreatures();
            auto it = creatures.begin();
            while (it != creatures.end()) {
                if (!(*it) || !(*it)->isAlive()) {
                    if (*it) {
                        stats.recordDeath((*it)->getSpecies());
                    }
                    it = creatures.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
}

void Simulator::save(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Не удалось создать файл сохранения.";
        return;
    }

    file << map.getSize() << "";
    file << stats.getCurrentDay() << "";

    auto creatures = map.getAllCreatures();
    file << creatures.size() << "";

    for (auto* c : creatures) {
        file << c->getSpecies() << " "
             << c->getHealth() << " "
             << c->getSatiety() << " "
             << c->getAge() << " "
             << c->getMaxAge() << " "
             << c->getSpeed() << " "
             << c->getStrength() << " "
             << c->getFertility() << " "
             << c->getX() << " "
             << c->getY() << "";
    }

    file.close();
    std::cout << "Игра сохранена в " << filename << "";
}

void Simulator::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Файл сохранения не найден.";
        return;
    }

    std::cout << "Загрузка сохранения...";
    running = true;
    file.close();
}

void Simulator::displayMap() {
    map.display();
}

void Simulator::displayStats() {
    stats.display();
}

void Simulator::displaySpeciesInfo(const std::string& species) {
    stats.displaySpecies(species);
}

void Simulator::addCreature(const std::string& type, int x, int y) {
    std::unique_ptr<Creature> creature;

    if (type == "herbivore") {
        creature = std::make_unique<Herbivore>();
    } else if (type == "predator") {
        creature = std::make_unique<Predator>();
    } else if (type == "omnivore") {
        creature = std::make_unique<Omnivore>();
    } else {
        std::cout << "Неизвестный тип существа.";
        return;
    }

    map.addCreature(std::move(creature), x, y);
    std::cout << "Существо добавлено на позицию (" << x << ", " << y << ")";
}

void Simulator::removeCreature(int id) {
    auto creatures = map.getAllCreatures();
    for (auto* c : creatures) {
        if (c && c->getId() == id) {
            int x = c->getX();
            int y = c->getY();
            Tile* tile = map.getTile(x, y);
            if (tile) {
                tile->removeCreature(id);
                std::cout << "Существо #" << id << " удалено.";
            }
            return;
        }
    }
    std::cout << "Существо с ID " << id << " не найдено.";
}

void Simulator::changeTerrain(int x, int y, int terrainType) {
    auto* tile = map.getTile(x, y);
    if (tile) {
        tile->setTerrain(static_cast<TerrainType>(terrainType));
        std::cout << "Тип местности изменён на клетке (" << x << ", " << y << ")";
    }
}

void Simulator::modifyPlants(int x, int y, int amount) {
    auto* tile = map.getTile(x, y);
    if (tile) {
        tile->setPlants(tile->getPlants() + amount);
        std::cout << "Количество растений изменено на клетке (" << x << ", " << y << ")";
    }
}
