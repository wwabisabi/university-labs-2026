#include "SalaryLogRepository.h"
#include "Exceptions.h"
#include <sstream>

SalaryLogRepository::SalaryLogRepository(DatabaseManager& db) : db(db) {}

std::vector<SalaryLog> SalaryLogRepository::getAll() {
    std::vector<SalaryLog> logs;

    PGresult* result = db.executeQuery(
        "SELECT id, employee_id, month_year, total_services_sum, salary_calculated, paid_status "
        "FROM salary_log ORDER BY id;"
    );

    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        logs.emplace_back(
            std::stoi(PQgetvalue(result, i, 0)),
            std::stoi(PQgetvalue(result, i, 1)),
            PQgetvalue(result, i, 2),
            std::stod(PQgetvalue(result, i, 3)),
            std::stod(PQgetvalue(result, i, 4)),
            std::string(PQgetvalue(result, i, 5)) == "t"
        );
    }

    PQclear(result);
    return logs;
}

SalaryLog SalaryLogRepository::getById(int id) {
    std::ostringstream query;
    query << "SELECT id, employee_id, month_year, total_services_sum, salary_calculated, paid_status "
          << "FROM salary_log WHERE id = " << id << ";";

    PGresult* result = db.executeQuery(query.str());

    if (PQntuples(result) == 0) {
        PQclear(result);
        throw NotFoundException("Salary log not found");
    }

    SalaryLog log(
        std::stoi(PQgetvalue(result, 0, 0)),
        std::stoi(PQgetvalue(result, 0, 1)),
        PQgetvalue(result, 0, 2),
        std::stod(PQgetvalue(result, 0, 3)),
        std::stod(PQgetvalue(result, 0, 4)),
        std::string(PQgetvalue(result, 0, 5)) == "t"
    );

    PQclear(result);
    return log;
}

void SalaryLogRepository::add(const SalaryLog& salary_log) {
    std::ostringstream query;
    query << "INSERT INTO salary_log (employee_id, month_year, total_services_sum, salary_calculated, paid_status) VALUES ("
          << salary_log.getEmployeeId() << ", '"
          << salary_log.getMonthYear() << "', "
          << salary_log.getTotalServicesSum() << ", "
          << salary_log.getSalaryCalculated() << ", "
          << (salary_log.getPaidStatus() ? "TRUE" : "FALSE") << ");";

    db.executeCommand(query.str());
}

void SalaryLogRepository::update(const SalaryLog& salary_log) {
    std::ostringstream query;
    query << "UPDATE salary_log SET "
          << "employee_id = " << salary_log.getEmployeeId() << ", "
          << "month_year = '" << salary_log.getMonthYear() << "', "
          << "total_services_sum = " << salary_log.getTotalServicesSum() << ", "
          << "salary_calculated = " << salary_log.getSalaryCalculated() << ", "
          << "paid_status = " << (salary_log.getPaidStatus() ? "TRUE" : "FALSE") << " "
          << "WHERE id = " << salary_log.getId() << ";";

    db.executeCommand(query.str());
}