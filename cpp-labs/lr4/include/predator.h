#ifndef PREDATOR_H
#define PREDATOR_H

#include "creature.h"

class Predator : public Creature {
public:
    Predator(double speed = 15, double strength = 20, double fertility = 0.15, int maxAge = 80);

    void move(Map& map) override;
    void eat(Map& map) override;
    std::unique_ptr<Creature> reproduce(const Creature& partner) override;

private:
    Predator(double health, double satiety, int age, int maxAge,
             double speed, double strength, double fertility);
};

#endif
