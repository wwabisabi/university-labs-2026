#ifndef MAP_H
#define MAP_H

#include "tile.h"
#include <vector>
#include <memory>

class Map {
private:
    int size;
    std::vector<std::vector<std::unique_ptr<Tile>>> grid;

public:
    Map(int size);

    void regenerateAllPlants();
    void addCreature(std::unique_ptr<Creature> creature, int x, int y);
    void moveCreature(Creature* creature, int newX, int newY);

    Tile* getTile(int x, int y);
    const Tile* getTile(int x, int y) const;

    Creature* findNearestPrey(Creature* predator);
    Tile* findNearestPlant(Creature* herbivore);

    void display() const;
    int getSize() const { return size; }

    std::vector<Creature*> getAllCreatures();

private:
    bool isValid(int x, int y) const;
    double distance(int x1, int y1, int x2, int y2) const;
};

#endif
