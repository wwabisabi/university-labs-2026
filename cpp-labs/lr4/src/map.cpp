#include "../include/map.h"
#include "../include/creature.h"
#include <iostream>
#include <cmath>
#include <random>
#include <iomanip>
#include <algorithm>

Map::Map(int size) : size(size) {
    grid.resize(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4);

    for (int i = 0; i < size; ++i) {
        grid[i].resize(size);
        for (int j = 0; j < size; ++j) {
            TerrainType terrain = static_cast<TerrainType>(dis(gen));
            grid[i][j] = std::make_unique<Tile>(i, j, terrain);
        }
    }
}

void Map::regenerateAllPlants() {
    for (auto& row : grid) {
        for (auto& tile : row) {
            tile->regeneratePlants();
        }
    }
}

void Map::addCreature(std::unique_ptr<Creature> creature, int x, int y) {
    if (isValid(x, y)) {
        creature->setPosition(x, y);
        grid[x][y]->addCreature(std::move(creature));
    }
}

void Map::moveCreature(Creature* creature, int newX, int newY) {
    if (!isValid(newX, newY)) return;

    int oldX = creature->getX();
    int oldY = creature->getY();

    if (oldX == newX && oldY == newY) return;

    auto& oldTile = grid[oldX][oldY]->getCreatures();
    auto it = std::find_if(oldTile.begin(), oldTile.end(),
        [creature](const std::unique_ptr<Creature>& c) {
            return c.get() == creature;
        });

    if (it != oldTile.end()) {
        auto creaturePtr = std::move(*it);
        oldTile.erase(it);

        creature->setPosition(newX, newY);
        grid[newX][newY]->addCreature(std::move(creaturePtr));
    }
}

Tile* Map::getTile(int x, int y) {
    if (isValid(x, y)) {
        return grid[x][y].get();
    }
    return nullptr;
}

const Tile* Map::getTile(int x, int y) const {
    if (isValid(x, y)) {
        return grid[x][y].get();
    }
    return nullptr;
}

Creature* Map::findNearestPrey(Creature* predator) {
    Creature* nearest = nullptr;
    double minDist = 1000000;

    int px = predator->getX();
    int py = predator->getY();

    for (auto& row : grid) {
        for (auto& tile : row) {
            for (auto& creature : tile->getCreatures()) {
                if (creature.get() != predator && 
                    (creature->getDietType() == DietType::HERBIVORE ||
                     creature->getDietType() == DietType::OMNIVORE)) {
                    double dist = distance(px, py, creature->getX(), creature->getY());
                    if (dist < minDist) {
                        minDist = dist;
                        nearest = creature.get();
                    }
                }
            }
        }
    }

    return nearest;
}

Tile* Map::findNearestPlant(Creature* herbivore) {
    Tile* nearest = nullptr;
    double minDist = 1000000;

    int hx = herbivore->getX();
    int hy = herbivore->getY();

    for (auto& row : grid) {
        for (auto& tile : row) {
            if (tile->getPlants() > 10) {
                double dist = distance(hx, hy, tile->getX(), tile->getY());
                if (dist < minDist) {
                    minDist = dist;
                    nearest = tile.get();
                }
            }
        }
    }

    return nearest;
}

void Map::display() const {
    std::cout << "\n  ";
    for (int i = 0; i < size; ++i) {
        std::cout << std::setw(2) << i;
    }
    std::cout << "\n";

    for (int i = 0; i < size; ++i) {
        std::cout << std::setw(2) << i << " ";
        for (int j = 0; j < size; ++j) {
            std::cout << grid[i][j]->getSymbol() << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nЛегенда: H-Травоядное, P-Хищник, O-Всеядное\n";
    std::cout << "         T-Лес, .-Поле, ~-Пустыня, W-Вода, ^-Горы\n";
}

std::vector<Creature*> Map::getAllCreatures() {
    std::vector<Creature*> all;
    for (auto& row : grid) {
        for (auto& tile : row) {
            for (auto& creature : tile->getCreatures()) {
                all.push_back(creature.get());
            }
        }
    }
    return all;
}

bool Map::isValid(int x, int y) const {
    return x >= 0 && x < size && y >= 0 && y < size;
}

double Map::distance(int x1, int y1, int x2, int y2) const {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
