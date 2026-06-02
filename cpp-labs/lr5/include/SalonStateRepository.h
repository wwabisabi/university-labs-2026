#ifndef SALON_STATE_REPOSITORY_H
#define SALON_STATE_REPOSITORY_H

#include <vector>
#include "SalonState.h"
#include "DatabaseManager.h"

class SalonStateRepository {
private:
    DatabaseManager& db;

public:
    explicit SalonStateRepository(DatabaseManager& db);

    std::vector<SalonState> getAll();
    SalonState getById(int id);
    void add(const SalonState& salonState);
    void update(const SalonState& salonState);
};

#endif