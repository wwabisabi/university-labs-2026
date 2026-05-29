#ifndef HERBIVORE_H
#define HERBIVORE_H

#include "creature.h"

class Herbivore : public Creature {
public:
    Herbivore(double speed = 10, double strength = 5, double fertility = 0.3, int maxAge = 50);

    void move(Map& map) override;
    void eat(Map& map) override;
    std::unique_ptr<Creature> reproduce(const Creature& partner) override;

private:
    Herbivore(double health, double satiety, int age, int maxAge, 
              double speed, double strength, double fertility);
};

#endif
