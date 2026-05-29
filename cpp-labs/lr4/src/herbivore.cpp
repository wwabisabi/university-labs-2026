#include "../include/herbivore.h"
#include "../include/map.h"
#include <random>
#include <cmath>

Herbivore::Herbivore(double speed, double strength, double fertility, int maxAge)
    : Creature("Травоядное", 80, 70, maxAge, speed, strength, fertility, DietType::HERBIVORE) {}

Herbivore::Herbivore(double health, double satiety, int age, int maxAge,
                     double speed, double strength, double fertility)
    : Creature("Травоядное", health, satiety, maxAge, speed, strength, fertility, DietType::HERBIVORE) {
    this->age = age;
}

void Herbivore::move(Map& map) {
    Tile* nearestPlant = map.findNearestPlant(this);
    if (nearestPlant && satiety < 50) {
        int targetX = nearestPlant->getX();
        int targetY = nearestPlant->getY();
        int dx = (targetX > x) ? 1 : (targetX < x) ? -1 : 0;
        int dy = (targetY > y) ? 1 : (targetY < y) ? -1 : 0;

        int newX = x + dx * static_cast<int>(speed / 10);
        int newY = y + dy * static_cast<int>(speed / 10);
        map.moveCreature(this, newX, newY);
    } else {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-1, 1);

        int newX = x + dis(gen);
        int newY = y + dis(gen);
        map.moveCreature(this, newX, newY);
    }
}

void Herbivore::eat(Map& map) {
    Tile* currentTile = map.getTile(x, y);
    if (currentTile && currentTile->getPlants() > 0) {
        currentTile->removePlants(20);
        increaseSatiety(30);
        setHealth(health + 5);
    }
}

std::unique_ptr<Creature> Herbivore::reproduce(const Creature& partner) {
    if (partner.getDietType() != DietType::HERBIVORE) return nullptr;
    if (health < 60 || satiety < 50 || partner.getHealth() < 60) return nullptr;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    if (dis(gen) > fertility) return nullptr;

    double newSpeed = mutate((speed + partner.getSpeed()) / 2.0);
    double newStrength = mutate((strength + partner.getStrength()) / 2.0);
    double newFertility = mutate((fertility + partner.getFertility()) / 2.0);
    int newMaxAge = static_cast<int>(mutate((maxAge + partner.getMaxAge()) / 2.0));

    auto child = std::make_unique<Herbivore>(newSpeed, newStrength, newFertility, newMaxAge);
    child->setPosition(x, y);

    decreaseSatiety(20);
    return child;
}
