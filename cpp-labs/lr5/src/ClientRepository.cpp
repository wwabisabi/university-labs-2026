#include "ClientRepository.h"
#include "Exceptions.h"
#include <sstream>

ClientRepository::ClientRepository(DatabaseManager& db) : db(db) {}

std::vector<Client> ClientRepository::getAll() {
    std::vector<Client> clients;

    PGresult* result = db.executeQuery(
        "SELECT id, full_name, phone, first_visit_date, total_spent, visits_count, bonus_points, is_deleted "
        "FROM clients WHERE is_deleted = FALSE ORDER BY id;"
    );

    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        clients.emplace_back(
            std::stoi(PQgetvalue(result, i, 0)),
            PQgetvalue(result, i, 1),
            PQgetvalue(result, i, 2),
            PQgetvalue(result, i, 3),
            std::stod(PQgetvalue(result, i, 4)),
            std::stoi(PQgetvalue(result, i, 5)),
            std::stoi(PQgetvalue(result, i, 6)),
            std::string(PQgetvalue(result, i, 7)) == "t"
        );
    }

    PQclear(result);
    return clients;
}

Client ClientRepository::getById(int id) {
    std::ostringstream query;
    query << "SELECT id, full_name, phone, first_visit_date, total_spent, visits_count, bonus_points, is_deleted "
          << "FROM clients WHERE id = " << id << " AND is_deleted = FALSE;";

    PGresult* result = db.executeQuery(query.str());

    if (PQntuples(result) == 0) {
        PQclear(result);
        throw NotFoundException("Client not found");
    }

    Client client(
        std::stoi(PQgetvalue(result, 0, 0)),
        PQgetvalue(result, 0, 1),
        PQgetvalue(result, 0, 2),
        PQgetvalue(result, 0, 3),
        std::stod(PQgetvalue(result, 0, 4)),
        std::stoi(PQgetvalue(result, 0, 5)),
        std::stoi(PQgetvalue(result, 0, 6)),
        std::string(PQgetvalue(result, 0, 7)) == "t"
    );

    PQclear(result);
    return client;
}

void ClientRepository::add(const Client& client) {
    std::ostringstream query;
    query << "INSERT INTO clients (full_name, phone, first_visit_date, total_spent, visits_count, bonus_points, is_deleted) VALUES ('"
          << client.getFullName() << "', '"
          << client.getPhone() << "', '"
          << client.getFirstVisitDate() << "', "
          << client.getTotalSpent() << ", "
          << client.getVisitsCount() << ", "
          << client.getBonusPoints() << ", "
          << (client.getIsDeleted() ? "TRUE" : "FALSE") << ");";

    db.executeCommand(query.str());
}

void ClientRepository::update(const Client& client) {
    std::ostringstream query;
    query << "UPDATE clients SET "
          << "full_name = '" << client.getFullName() << "', "
          << "phone = '" << client.getPhone() << "', "
          << "first_visit_date = '" << client.getFirstVisitDate() << "', "
          << "total_spent = " << client.getTotalSpent() << ", "
          << "visits_count = " << client.getVisitsCount() << ", "
          << "bonus_points = " << client.getBonusPoints() << ", "
          << "is_deleted = " << (client.getIsDeleted() ? "TRUE" : "FALSE") << " "
          << "WHERE id = " << client.getId() << ";";

    db.executeCommand(query.str());
}

void ClientRepository::softDelete(int id) {
    std::ostringstream query;
    query << "UPDATE clients SET is_deleted = TRUE WHERE id = " << id << ";";
    db.executeCommand(query.str());
}