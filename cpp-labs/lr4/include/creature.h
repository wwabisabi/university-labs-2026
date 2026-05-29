#ifndef CREATURE_H
#define CREATURE_H

#include <string>
#include <memory>
#include <random>

enum class DietType {
    HERBIVORE,
    PREDATOR,
    OMNIVORE
};

class Map;
class Tile;

class Creature {
protected:
    static int nextId;
    static int totalCreated;

    int id;
    std::string species;
    double health;
    double satiety;
    int age;
    int maxAge;
    double speed;
    double strength;
    double fertility;
    DietType dietType;
    int x, y;

public:
    Creature(const std::string& species, double health, double satiety, 
             int maxAge, double speed, double strength, double fertility, DietType diet);
    Creature(const Creature& other);
    Creature& operator=(const Creature& other);
    virtual ~Creature();

    virtual void move(Map& map) = 0;
    virtual void eat(Map& map) = 0;
    virtual std::unique_ptr<Creature> reproduce(const Creature& partner) = 0;

    void increaseAge();
    void decreaseHealth(double amount);
    void increaseSatiety(double amount);
    void decreaseSatiety(double amount);

    virtual std::string getInfo() const;
    bool isAlive() const;

    int getId() const { return id; }
    std::string getSpecies() const { return species; }
    double getHealth() const { return health; }
    double getSatiety() const { return satiety; }
    int getAge() const { return age; }
    int getMaxAge() const { return maxAge; }
    double getSpeed() const { return speed; }
    double getStrength() const { return strength; }
    double getFertility() const { return fertility; }
    DietType getDietType() const { return dietType; }
    int getX() const { return x; }
    int getY() const { return y; }

    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void setHealth(double h) { health = std::max(0.0, std::min(100.0, h)); }

    static int getTotalCreated() { return totalCreated; }

protected:
    double mutate(double value, double range = 0.1);
};

#endif
