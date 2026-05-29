#include "../include/statistics.h"
#include "../include/creature.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

Statistics::Statistics() : currentDay(0) {}

void Statistics::update(const std::vector<Creature*>& creatures) {
    stats.clear();

    for (auto* creature : creatures) {
        std::string species = creature->getSpecies();
        auto& s = stats[species];

        s.population++;
        s.avgSpeed += creature->getSpeed();
        s.avgStrength += creature->getStrength();
        s.avgFertility += creature->getFertility();
        s.avgAge += creature->getAge();

        s.minSpeed = std::min(s.minSpeed, creature->getSpeed());
        s.maxSpeed = std::max(s.maxSpeed, creature->getSpeed());
        s.minStrength = std::min(s.minStrength, creature->getStrength());
        s.maxStrength = std::max(s.maxStrength, creature->getStrength());
    }

    for (auto& [species, s] : stats) {
        if (s.population > 0) {
            s.avgSpeed /= s.population;
            s.avgStrength /= s.population;
            s.avgFertility /= s.population;
            s.avgAge /= s.population;
        }
    }

    std::map<std::string, int> dayPopulation;
    for (const auto& [species, s] : stats) {
        dayPopulation[species] = s.population;
    }
    populationHistory.push_back(dayPopulation);
}

void Statistics::recordBirth(const std::string& species) {
    stats[species].births++;
}

void Statistics::recordDeath(const std::string& species) {
    stats[species].deaths++;
}

void Statistics::display() const {
    std::cout << "\n========== СТАТИСТИКА (День " << currentDay << ") ==========\n";

    for (const auto& [species, s] : stats) {
        std::cout << "\n" << species << ":\n";
        std::cout << "  Популяция: " << s.population << "\n";
        std::cout << "  Рождений: " << s.births << " | Смертей: " << s.deaths << "\n";
        std::cout << "  Средняя скорость: " << std::fixed << std::setprecision(1) << s.avgSpeed << "\n";
        std::cout << "  Средняя сила: " << s.avgStrength << "\n";
        std::cout << "  Средняя плодовитость: " << std::setprecision(2) << s.avgFertility << "\n";
        std::cout << "  Средний возраст: " << std::setprecision(1) << s.avgAge << "\n";
    }

    int totalPopulation = 0;
    for (const auto& [species, s] : stats) {
        totalPopulation += s.population;
    }
    std::cout << "\nОБЩАЯ ПОПУЛЯЦИЯ: " << totalPopulation << "\n";
}

void Statistics::displaySpecies(const std::string& species) const {
    auto it = stats.find(species);
    if (it == stats.end()) {
        std::cout << "Вид \"" << species << "\" не найден.\n";
        return;
    }

    const auto& s = it->second;
    std::cout << "\n========== " << species << " ==========\n";
    std::cout << "Популяция: " << s.population << "\n";
    std::cout << "Рождений: " << s.births << " | Смертей: " << s.deaths << "\n";
    std::cout << "\nХарактеристики:\n";
    std::cout << "  Скорость: " << std::fixed << std::setprecision(1) 
              << s.avgSpeed << " (мин: " << s.minSpeed << ", макс: " << s.maxSpeed << ")\n";
    std::cout << "  Сила: " << s.avgStrength 
              << " (мин: " << s.minStrength << ", макс: " << s.maxStrength << ")\n";
    std::cout << "  Плодовитость: " << std::setprecision(2) << s.avgFertility << "\n";
    std::cout << "  Средний возраст: " << std::setprecision(1) << s.avgAge << "\n";
}

void Statistics::exportToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл " << filename << "\n";
        return;
    }

    file << "День";
    for (const auto& [species, _] : stats) {
        file << "," << species;
    }
    file << "\n";

    for (size_t day = 0; day < populationHistory.size(); ++day) {
        file << day;
        for (const auto& [species, _] : stats) {
            auto it = populationHistory[day].find(species);
            file << "," << (it != populationHistory[day].end() ? it->second : 0);
        }
        file << "\n";
    }

    file.close();
    std::cout << "Статистика экспортирована в " << filename << "\n";
}

void Statistics::nextDay() {
    currentDay++;
}
