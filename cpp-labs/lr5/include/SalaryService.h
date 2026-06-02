#ifndef SALARY_SERVICE_H
#define SALARY_SERVICE_H

#include <string>
#include "EmployeeRepository.h"
#include "SalaryLogRepository.h"
#include "AppointmentRepository.h"
#include "FinanceService.h"

class SalaryService {
private:
    EmployeeRepository& employee_repository;
    SalaryLogRepository& salary_log_repository;
    AppointmentRepository& appointment_repository;
    FinanceService& finance_service;

    double calculate_employee_services_sum(int employee_id);
    std::string get_current_month_key();

public:
    SalaryService(EmployeeRepository& employee_repository,
                  SalaryLogRepository& salary_log_repository,
                  AppointmentRepository& appointment_repository,
                  FinanceService& finance_service);

    double calculate_salary_for_employee(int employee_id);
    void calculate_and_save_all_salaries();
    void print_salary_logs();
};

#endif