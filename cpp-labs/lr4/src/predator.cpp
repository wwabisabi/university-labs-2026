#include "../include/predator.h"
#include "../include/map.h"
#include <random>

Predator::Predator(double speed, double strength, double fertility, int maxAge)
    : Creature("Хищник", 90, 60, maxAge, speed, strength, fertility, DietType::PREDATOR) {}

Predator::Predator(double health, double satiety, int age, int maxAge,
                   double speed, double strength, double fertility)
    : Creature("Хищник", health, satiety, maxAge, speed, strength, fertility, DietType::PREDATOR) {
    this->age = age;
}

void Predator::move(Map& map) {
    Creature* prey = map.findNearestPrey(this);
    if (prey && satiety < 40) {
        int targetX = prey->getX();
        int targetY = prey->getY();
        int dx = (targetX > x) ? 1 : (targetX < x) ? -1 : 0;
        int dy = (targetY > y) ? 1 : (targetY < y) ? -1 : 0;

        int step = std::max(1, static_cast<int>(speed / 8));
        int newX = x + dx * step;
        int newY = y + dy * step;
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

void Predator::eat(Map& map) {
    Tile* tile = map.getTile(x, y);
    if (!tile) return;

    auto& creatures = tile->getCreatures();
    for (auto it = creatures.begin(); it != creatures.end(); ) {
        Creature* prey = it->get();
        if (prey != this &&
            (prey->getDietType() == DietType::HERBIVORE ||
             prey->getDietType() == DietType::OMNIVORE) &&
            strength > prey->getStrength()) {
            increaseSatiety(50);
            setHealth(health + 10);
            it = creatures.erase(it);
            return;
        } else {
            ++it;
        }
    }
}

std::unique_ptr<Creature> Predator::reproduce(const Creature& partner) {
    if (partner.getDietType() != DietType::PREDATOR) return nullptr;
    if (health < 70 || satiety < 60 || partner.getHealth() < 70) return nullptr;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    if (dis(gen) > fertility) return nullptr;

    double newSpeed = mutate((speed + partner.getSpeed()) / 2.0);
    double newStrength = mutate((strength + partner.getStrength()) / 2.0);
    double newFertility = mutate((fertility + partner.getFertility()) / 2.0);
    int newMaxAge = static_cast<int>(mutate((maxAge + partner.getMaxAge()) / 2.0));

    auto child = std::make_unique<Predator>(newSpeed, newStrength, newFertility, newMaxAge);
    child->setPosition(x, y);

    decreaseSatiety(30);
    return child;
}
