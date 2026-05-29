#ifndef OMNIVORE_H
#define OMNIVORE_H

#include "creature.h"

class Omnivore : public Creature {
public:
    Omnivore(double speed = 12, double strength = 12, double fertility = 0.22, int maxAge = 65);

    void move(Map& map) override;
    void eat(Map& map) override;
    std::unique_ptr<Creature> reproduce(const Creature& partner) override;

private:
    Omnivore(double health, double satiety, int age, int maxAge,
             double speed, double strength, double fertility);
};

#endif
