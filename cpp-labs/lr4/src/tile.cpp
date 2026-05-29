#include "../include/tile.h"
#include "../include/creature.h"
#include <algorithm>

Tile::Tile(int x, int y, TerrainType terrain)
    : x(x), y(y), terrain(terrain), plants(0) {
    switch (terrain) {
        case TerrainType::FOREST: plants = 80; break;
        case TerrainType::FIELD: plants = 60; break;
        case TerrainType::DESERT: plants = 10; break;
        case TerrainType::WATER: plants = 30; break;
        case TerrainType::MOUNTAIN: plants = 5; break;
    }
}

void Tile::addCreature(std::unique_ptr<Creature> creature) {
    creatures.push_back(std::move(creature));
}

void Tile::removeCreature(int creatureId) {
    creatures.erase(
        std::remove_if(creatures.begin(), creatures.end(),
            [creatureId](const std::unique_ptr<Creature>& c) {
                return c->getId() == creatureId;
            }),
        creatures.end()
    );
}

void Tile::regeneratePlants() {
    int maxPlants = 0;
    int regen = 0;

    switch (terrain) {
        case TerrainType::FOREST:
            maxPlants = 100;
            regen = 8;
            break;
        case TerrainType::FIELD:
            maxPlants = 80;
            regen = 10;
            break;
        case TerrainType::DESERT:
            maxPlants = 20;
            regen = 2;
            break;
        case TerrainType::WATER:
            maxPlants = 40;
            regen = 5;
            break;
        case TerrainType::MOUNTAIN:
            maxPlants = 10;
            regen = 1;
            break;
    }

    plants = std::min(plants + regen, maxPlants);
}

void Tile::removePlants(int amount) {
    plants = std::max(0, plants - amount);
}

char Tile::getSymbol() const {
    if (!creatures.empty()) {
        auto& c = creatures[0];
        switch (c->getDietType()) {
            case DietType::HERBIVORE: return 'H';
            case DietType::PREDATOR: return 'P';
            case DietType::OMNIVORE: return 'O';
        }
    }

    switch (terrain) {
        case TerrainType::FOREST: return 'T';
        case TerrainType::FIELD: return '.';
        case TerrainType::DESERT: return '~';
        case TerrainType::WATER: return 'W';
        case TerrainType::MOUNTAIN: return '^';
    }
    return '?';
}

std::string Tile::getTerrainName() const {
    switch (terrain) {
        case TerrainType::FOREST: return "Лес";
        case TerrainType::FIELD: return "Поле";
        case TerrainType::DESERT: return "Пустыня";
        case TerrainType::WATER: return "Вода";
        case TerrainType::MOUNTAIN: return "Горы";
    }
    return "Неизвестно";
}
