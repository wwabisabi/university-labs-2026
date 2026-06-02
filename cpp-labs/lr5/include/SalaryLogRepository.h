#ifndef SALARY_LOG_REPOSITORY_H
#define SALARY_LOG_REPOSITORY_H

#include <vector>
#include "SalaryLog.h"
#include "DatabaseManager.h"

class SalaryLogRepository {
private:
    DatabaseManager& db;

public:
    explicit SalaryLogRepository(DatabaseManager& db);

    std::vector<SalaryLog> getAll();
    SalaryLog getById(int id);
    void add(const SalaryLog& salaryLog);
    void update(const SalaryLog& salaryLog);
};

#endif