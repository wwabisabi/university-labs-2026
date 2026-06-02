#ifndef EMPLOYEE_REPOSITORY_H
#define EMPLOYEE_REPOSITORY_H

#include <vector>
#include "Employee.h"
#include "DatabaseManager.h"

class EmployeeRepository {
private:
    DatabaseManager& db;

public:
    explicit EmployeeRepository(DatabaseManager& db);

    std::vector<Employee> getAll();
    Employee getById(int id);
    void add(const Employee& employee);
    void update(const Employee& employee);
    void softDelete(int id);
};

#endif