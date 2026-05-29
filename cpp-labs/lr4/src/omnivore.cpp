#include "../include/omnivore.h"
#include "../include/map.h"
#include <random>

Omnivore::Omnivore(double speed, double strength, double fertility, int maxAge)
    : Creature("Всеядное", 85, 65, maxAge, speed, strength, fertility, DietType::OMNIVORE) {}

Omnivore::Omnivore(double health, double satiety, int age, int maxAge,
                   double speed, double strength, double fertility)
    : Creature("Всеядное", health, satiety, maxAge, speed, strength, fertility, DietType::OMNIVORE) {
    this->age = age;
}

void Omnivore::move(Map& map) {
    if (satiety < 50) {
        Tile* nearestPlant = map.findNearestPlant(this);
        Creature* nearestPrey = map.findNearestPrey(this);

        if (nearestPlant && (!nearestPrey || satiety < 30)) {
            int targetX = nearestPlant->getX();
            int targetY = nearestPlant->getY();
            int dx = (targetX > x) ? 1 : (targetX < x) ? -1 : 0;
            int dy = (targetY > y) ? 1 : (targetY < y) ? -1 : 0;

            int step = std::max(1, static_cast<int>(speed / 10));
            int newX = x + dx * step;
            int newY = y + dy * step;
            map.moveCreature(this, newX, newY);
        } else if (nearestPrey) {
            int targetX = nearestPrey->getX();
            int targetY = nearestPrey->getY();
            int dx = (targetX > x) ? 1 : (targetX < x) ? -1 : 0;
            int dy = (targetY > y) ? 1 : (targetY < y) ? -1 : 0;

            int step = std::max(1, static_cast<int>(speed / 10));
            int newX = x + dx * step;
            int newY = y + dy * step;
            map.moveCreature(this, newX, newY);
        }
    } else {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-1, 1);

        int newX = x + dis(gen);
        int newY = y + dis(gen);
        map.moveCreature(this, newX, newY);
    }
}

void Omnivore::eat(Map& map) {
    Tile* currentTile = map.getTile(x, y);
    if (!currentTile) return;

    if (satiety < 40) {
        auto& creatures = currentTile->getCreatures();
        for (auto it = creatures.begin(); it != creatures.end(); ) {
            Creature* prey = it->get();
            if (prey != this &&
                prey->getDietType() == DietType::HERBIVORE &&
                strength > prey->getStrength() * 0.8) {
                increaseSatiety(40);
                setHealth(health + 8);
                it = creatures.erase(it);
                return;
            } else {
                ++it;
            }
        }
    }

    if (currentTile->getPlants() > 0 && satiety < 70) {
        currentTile->removePlants(15);
        increaseSatiety(20);
        setHealth(health + 3);
    }
}

std::unique_ptr<Creature> Omnivore::reproduce(const Creature& partner) {
    if (partner.getDietType() != DietType::OMNIVORE) return nullptr;
    if (health < 65 || satiety < 55 || partner.getHealth() < 65) return nullptr;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    if (dis(gen) > fertility) return nullptr;

    double newSpeed = mutate((speed + partner.getSpeed()) / 2.0);
    double newStrength = mutate((strength + partner.getStrength()) / 2.0);
    double newFertility = mutate((fertility + partner.getFertility()) / 2.0);
    int newMaxAge = static_cast<int>(mutate((maxAge + partner.getMaxAge()) / 2.0));

    auto child = std::make_unique<Omnivore>(newSpeed, newStrength, newFertility, newMaxAge);
    child->setPosition(x, y);

    decreaseSatiety(25);
    return child;
}
