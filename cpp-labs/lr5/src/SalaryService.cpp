#include "SalaryService.h"
#include "Exceptions.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

SalaryService::SalaryService(EmployeeRepository& employee_repository,
                             SalaryLogRepository& salary_log_repository,
                             AppointmentRepository& appointment_repository,
                             FinanceService& finance_service)
    : employee_repository(employee_repository),
      salary_log_repository(salary_log_repository),
      appointment_repository(appointment_repository),
      finance_service(finance_service) {}

double SalaryService::calculate_employee_services_sum(int employee_id) {
    std::vector<Appointment> appointments = appointment_repository.getAll();
    double total = 0.0;

    for (const auto& appointment : appointments) {
        if (appointment.getEmployeeId() == employee_id &&
            appointment.getStatus() == "completed") {
            total += appointment.getPriceAtBooking();
        }
    }

    return total;
}

std::string SalaryService::get_current_month_key() {
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    std::ostringstream out;
    out << std::put_time(local_time, "%Y-%m-01");
    return out.str();
}

double SalaryService::calculate_salary_for_employee(int employee_id) {
    Employee employee = employee_repository.getById(employee_id);
    double total_services_sum = calculate_employee_services_sum(employee_id);

    return employee.getSalaryBase() +
           total_services_sum * employee.getSalaryPercent() / 100.0;
}

void SalaryService::calculate_and_save_all_salaries() {
    std::vector<Employee> employees = employee_repository.getAll();
    std::string month_key = get_current_month_key();

    for (const auto& employee : employees) {
        double total_services_sum = calculate_employee_services_sum(employee.getId());
        double salary = employee.getSalaryBase() +
                        total_services_sum * employee.getSalaryPercent() / 100.0;

        SalaryLog log(
            0,
            employee.getId(),
            month_key,
            total_services_sum,
            salary,
            true
        );

        salary_log_repository.add(log);
        finance_service.add_expense(salary);
    }
}

void SalaryService::print_salary_logs() {
    std::vector<SalaryLog> logs = salary_log_repository.getAll();
    for (const auto& log : logs) {
        std::cout << log.toString() << std::endl;
    }
}