#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "map.h"
#include "statistics.h"
#include <string>

class Simulator {
private:
    Map map;
    Statistics stats;
    bool running;
    bool autoMode;
    int timeSpeed;

public:
    Simulator(int mapSize);

    void initialize(int herbivores, int predators, int omnivores);
    void step();
    void stepMonth();
    void run();

    void save(const std::string& filename);
    void load(const std::string& filename);

    void displayMap();
    void displayStats();
    void displaySpeciesInfo(const std::string& species);

    void addCreature(const std::string& type, int x, int y);
    void removeCreature(int id);
    void changeTerrain(int x, int y, int terrainType);
    void modifyPlants(int x, int y, int amount);

    bool isRunning() const { return running; }
    void stop() { running = false; }

    void setAutoMode(bool mode) { autoMode = mode; }
    void increaseSpeed() { timeSpeed = std::min(10, timeSpeed + 1); }
    void decreaseSpeed() { timeSpeed = std::max(1, timeSpeed - 1); }

private:
    void updateResources();
    void processCreatures();
    void updateStates();
    void handleReproduction();
    void removeDeadCreatures();
};

#endif
