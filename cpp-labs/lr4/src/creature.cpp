#include "../include/creature.h"
#include <iostream>
#include <cmath>

int Creature::nextId = 1;
int Creature::totalCreated = 0;

Creature::Creature(const std::string& species, double health, double satiety,
                   int maxAge, double speed, double strength, double fertility, DietType diet)
    : id(nextId++), species(species), health(health), satiety(satiety),
      age(0), maxAge(maxAge), speed(speed), strength(strength),
      fertility(fertility), dietType(diet), x(0), y(0) {
    totalCreated++;
    std::cout << "Существо #" << id << " (" << species << ") родилось!\n";
}

Creature::Creature(const Creature& other)
    : id(nextId++), species(other.species), health(other.health),
      satiety(other.satiety), age(other.age), maxAge(other.maxAge),
      speed(other.speed), strength(other.strength), fertility(other.fertility),
      dietType(other.dietType), x(other.x), y(other.y) {
    totalCreated++;
}

Creature& Creature::operator=(const Creature& other) {
    if (this != &other) {
        species = other.species;
        health = other.health;
        satiety = other.satiety;
        age = other.age;
        maxAge = other.maxAge;
        speed = other.speed;
        strength = other.strength;
        fertility = other.fertility;
        dietType = other.dietType;
        x = other.x;
        y = other.y;
    }
    return *this;
}

Creature::~Creature() {
    std::cout << "Существо #" << id << " (" << species << ") умерло в возрасте " 
              << age << " ходов.\n";
}

void Creature::increaseAge() {
    age++;
    decreaseSatiety(5 + age * 0.1);
    if (age > maxAge * 0.8) {
        decreaseHealth(2);
    }
}

void Creature::decreaseHealth(double amount) {
    health = std::max(0.0, health - amount);
}

void Creature::increaseSatiety(double amount) {
    satiety = std::min(100.0, satiety + amount);
}

void Creature::decreaseSatiety(double amount) {
    satiety = std::max(0.0, satiety - amount);
    if (satiety < 20) {
        decreaseHealth(3);
    }
}

std::string Creature::getInfo() const {
    return "ID: " + std::to_string(id) + " | " + species +
           " | Здоровье: " + std::to_string(static_cast<int>(health)) +
           " | Сытость: " + std::to_string(static_cast<int>(satiety)) +
           " | Возраст: " + std::to_string(age) + "/" + std::to_string(maxAge) +
           " | Скорость: " + std::to_string(static_cast<int>(speed)) +
           " | Сила: " + std::to_string(static_cast<int>(strength)) +
           " | Позиция: (" + std::to_string(x) + "," + std::to_string(y) + ")";
}

bool Creature::isAlive() const {
    return health > 0 && age <= maxAge;
}

double Creature::mutate(double value, double range) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-range, range);
    return value * (1.0 + dis(gen));
}
