#ifndef STATISTICS_H
#define STATISTICS_H

#include <string>
#include <map>
#include <vector>

struct SpeciesStats {
    int population = 0;
    int births = 0;
    int deaths = 0;
    double avgSpeed = 0.0;
    double avgStrength = 0.0;
    double avgFertility = 0.0;
    double avgAge = 0.0;
    double minSpeed = 1000.0;
    double maxSpeed = 0.0;
    double minStrength = 1000.0;
    double maxStrength = 0.0;
};

class Statistics {
private:
    std::map<std::string, SpeciesStats> stats;
    std::vector<std::map<std::string, int>> populationHistory;
    int currentDay;

public:
    Statistics();

    void update(const std::vector<class Creature*>& creatures);
    void recordBirth(const std::string& species);
    void recordDeath(const std::string& species);
    void display() const;
    void displaySpecies(const std::string& species) const;
    void exportToCSV(const std::string& filename) const;
    void nextDay();

    int getCurrentDay() const { return currentDay; }
    const std::map<std::string, SpeciesStats>& getStats() const { return stats; }
};

#endif
