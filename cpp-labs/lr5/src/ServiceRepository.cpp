#include "ServiceRepository.h"
#include "Exceptions.h"
#include <sstream>

ServiceRepository::ServiceRepository(DatabaseManager& db) : db(db) {}

std::vector<Service> ServiceRepository::getAll() {
    std::vector<Service> services;

    PGresult* result = db.executeQuery(
        "SELECT id, name, duration_minutes, price, required_specialization, material_cost "
        "FROM services ORDER BY id;"
    );

    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        services.emplace_back(
            std::stoi(PQgetvalue(result, i, 0)),
            PQgetvalue(result, i, 1),
            std::stoi(PQgetvalue(result, i, 2)),
            std::stod(PQgetvalue(result, i, 3)),
            PQgetvalue(result, i, 4),
            std::stod(PQgetvalue(result, i, 5))
        );
    }

    PQclear(result);
    return services;
}

Service ServiceRepository::getById(int id) {
    std::ostringstream query;
    query << "SELECT id, name, duration_minutes, price, required_specialization, material_cost "
          << "FROM services WHERE id = " << id << ";";

    PGresult* result = db.executeQuery(query.str());

    if (PQntuples(result) == 0) {
        PQclear(result);
        throw NotFoundException("Service not found");
    }

    Service service(
        std::stoi(PQgetvalue(result, 0, 0)),
        PQgetvalue(result, 0, 1),
        std::stoi(PQgetvalue(result, 0, 2)),
        std::stod(PQgetvalue(result, 0, 3)),
        PQgetvalue(result, 0, 4),
        std::stod(PQgetvalue(result, 0, 5))
    );

    PQclear(result);
    return service;
}

void ServiceRepository::add(const Service& service) {
    std::ostringstream query;
    query << "INSERT INTO services (name, duration_minutes, price, required_specialization, material_cost) VALUES ('"
          << service.getName() << "', "
          << service.getDurationMinutes() << ", "
          << service.getPrice() << ", '"
          << service.getRequiredSpecialization() << "', "
          << service.getMaterialCost() << ");";

    db.executeCommand(query.str());
}

void ServiceRepository::update(const Service& service) {
    std::ostringstream query;
    query << "UPDATE services SET "
          << "name = '" << service.getName() << "', "
          << "duration_minutes = " << service.getDurationMinutes() << ", "
          << "price = " << service.getPrice() << ", "
          << "required_specialization = '" << service.getRequiredSpecialization() << "', "
          << "material_cost = " << service.getMaterialCost() << " "
          << "WHERE id = " << service.getId() << ";";

    db.executeCommand(query.str());
}