#include "EmployeeRepository.h"
#include "Exceptions.h"
#include <sstream>

EmployeeRepository::EmployeeRepository(DatabaseManager& db) : db(db) {}

std::vector<Employee> EmployeeRepository::getAll() {
    std::vector<Employee> employees;

    PGresult* result = db.executeQuery(
        "SELECT id, full_name, specialization, experience, rating, salary_base, "
        "salary_percent, status, is_deleted "
        "FROM employees WHERE is_deleted = FALSE ORDER BY id;"
    );

    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        employees.emplace_back(
            std::stoi(PQgetvalue(result, i, 0)),
            PQgetvalue(result, i, 1),
            PQgetvalue(result, i, 2),
            std::stoi(PQgetvalue(result, i, 3)),
            std::stod(PQgetvalue(result, i, 4)),
            std::stod(PQgetvalue(result, i, 5)),
            std::stod(PQgetvalue(result, i, 6)),
            PQgetvalue(result, i, 7),
            std::string(PQgetvalue(result, i, 8)) == "t"
        );
    }

    PQclear(result);
    return employees;
}

Employee EmployeeRepository::getById(int id) {
    std::ostringstream query;
    query << "SELECT id, full_name, specialization, experience, rating, salary_base, "
          << "salary_percent, status, is_deleted "
          << "FROM employees WHERE id = " << id << " AND is_deleted = FALSE;";

    PGresult* result = db.executeQuery(query.str());

    if (PQntuples(result) == 0) {
        PQclear(result);
        throw NotFoundException("Employee not found");
    }

    Employee employee(
        std::stoi(PQgetvalue(result, 0, 0)),
        PQgetvalue(result, 0, 1),
        PQgetvalue(result, 0, 2),
        std::stoi(PQgetvalue(result, 0, 3)),
        std::stod(PQgetvalue(result, 0, 4)),
        std::stod(PQgetvalue(result, 0, 5)),
        std::stod(PQgetvalue(result, 0, 6)),
        PQgetvalue(result, 0, 7),
        std::string(PQgetvalue(result, 0, 8)) == "t"
    );

    PQclear(result);
    return employee;
}

void EmployeeRepository::add(const Employee& employee) {
    std::ostringstream query;
    query << "INSERT INTO employees (full_name, specialization, experience, rating, salary_base, salary_percent, status, is_deleted) VALUES ('"
          << employee.getFullName() << "', '"
          << employee.getSpecialization() << "', "
          << employee.getExperience() << ", "
          << employee.getRating() << ", "
          << employee.getSalaryBase() << ", "
          << employee.getSalaryPercent() << ", '"
          << employee.getStatus() << "', "
          << (employee.getIsDeleted() ? "TRUE" : "FALSE") << ");";

    db.executeCommand(query.str());
}

void EmployeeRepository::update(const Employee& employee) {
    std::ostringstream query;
    query << "UPDATE employees SET "
          << "full_name = '" << employee.getFullName() << "', "
          << "specialization = '" << employee.getSpecialization() << "', "
          << "experience = " << employee.getExperience() << ", "
          << "rating = " << employee.getRating() << ", "
          << "salary_base = " << employee.getSalaryBase() << ", "
          << "salary_percent = " << employee.getSalaryPercent() << ", "
          << "status = '" << employee.getStatus() << "', "
          << "is_deleted = " << (employee.getIsDeleted() ? "TRUE" : "FALSE") << " "
          << "WHERE id = " << employee.getId() << ";";

    db.executeCommand(query.str());
}

void EmployeeRepository::softDelete(int id) {
    std::ostringstream query;
    query << "UPDATE employees SET is_deleted = TRUE WHERE id = " << id << ";";
    db.executeCommand(query.str());
}