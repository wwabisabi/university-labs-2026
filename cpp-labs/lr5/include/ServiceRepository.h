#ifndef SERVICE_REPOSITORY_H
#define SERVICE_REPOSITORY_H

#include <vector>
#include "Service.h"
#include "DatabaseManager.h"

class ServiceRepository {
private:
    DatabaseManager& db;

public:
    explicit ServiceRepository(DatabaseManager& db);

    std::vector<Service> getAll();
    Service getById(int id);
    void add(const Service& service);
    void update(const Service& service);
};

#endif