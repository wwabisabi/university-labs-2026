#include "SalonStateRepository.h"
#include "Exceptions.h"
#include <sstream>

SalonStateRepository::SalonStateRepository(DatabaseManager& db) : db(db) {}

std::vector<SalonState> SalonStateRepository::getAll() {
    std::vector<SalonState> states;

    PGresult* result = db.executeQuery(
        "SELECT id, current_salon_date, cash_balance, last_month_salary_paid, total_revenue, total_expenses "
        "FROM salon_state ORDER BY id;"
    );

    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        states.emplace_back(
            std::stoi(PQgetvalue(result, i, 0)),
            PQgetvalue(result, i, 1),
            std::stod(PQgetvalue(result, i, 2)),
            PQgetvalue(result, i, 3),
            std::stod(PQgetvalue(result, i, 4)),
            std::stod(PQgetvalue(result, i, 5))
        );
    }

    PQclear(result);
    return states;
}

SalonState SalonStateRepository::getById(int id) {
    std::ostringstream query;
    query << "SELECT id, current_salon_date, cash_balance, last_month_salary_paid, total_revenue, total_expenses "
          << "FROM salon_state WHERE id = " << id << ";";

    PGresult* result = db.executeQuery(query.str());

    if (PQntuples(result) == 0) {
        PQclear(result);
        throw NotFoundException("Salon state not found");
    }

    SalonState state(
        std::stoi(PQgetvalue(result, 0, 0)),
        PQgetvalue(result, 0, 1),
        std::stod(PQgetvalue(result, 0, 2)),
        PQgetvalue(result, 0, 3),
        std::stod(PQgetvalue(result, 0, 4)),
        std::stod(PQgetvalue(result, 0, 5))
    );

    PQclear(result);
    return state;
}

void SalonStateRepository::add(const SalonState& salon_state) {
    std::ostringstream query;
    query << "INSERT INTO salon_state (current_salon_date, cash_balance, last_month_salary_paid, total_revenue, total_expenses) VALUES ('"
          << salon_state.getCurrentSalonDate() << "', "
          << salon_state.getCashBalance() << ", '"
          << salon_state.getLastMonthSalaryPaid() << "', "
          << salon_state.getTotalRevenue() << ", "
          << salon_state.getTotalExpenses() << ");";

    db.executeCommand(query.str());
}

void SalonStateRepository::update(const SalonState& salon_state) {
    std::ostringstream query;
    query << "UPDATE salon_state SET "
          << "current_salon_date = '" << salon_state.getCurrentSalonDate() << "', "
          << "cash_balance = " << salon_state.getCashBalance() << ", "
          << "last_month_salary_paid = '" << salon_state.getLastMonthSalaryPaid() << "', "
          << "total_revenue = " << salon_state.getTotalRevenue() << ", "
          << "total_expenses = " << salon_state.getTotalExpenses() << " "
          << "WHERE id = " << salon_state.getId() << ";";

    db.executeCommand(query.str());
}