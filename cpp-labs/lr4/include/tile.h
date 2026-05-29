#ifndef TILE_H
#define TILE_H

#include <vector>
#include <memory>
#include <string>

class Creature;

enum class TerrainType {
    FOREST,
    FIELD,
    DESERT,
    WATER,
    MOUNTAIN
};

class Tile {
private:
    int x, y;
    TerrainType terrain;
    int plants;
    std::vector<std::unique_ptr<Creature>> creatures;

public:
    Tile(int x, int y, TerrainType terrain);

    void addCreature(std::unique_ptr<Creature> creature);
    void removeCreature(int creatureId);
    void regeneratePlants();
    void removePlants(int amount);

    int getX() const { return x; }
    int getY() const { return y; }
    TerrainType getTerrain() const { return terrain; }
    int getPlants() const { return plants; }
    std::vector<std::unique_ptr<Creature>>& getCreatures() { return creatures; }
    const std::vector<std::unique_ptr<Creature>>& getCreatures() const { return creatures; }

    void setTerrain(TerrainType t) { terrain = t; }
    void setPlants(int p) { plants = std::max(0, std::min(100, p)); }

    char getSymbol() const;
    std::string getTerrainName() const;
};

#endif
