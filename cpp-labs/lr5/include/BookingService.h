#ifndef BOOKING_SERVICE_H
#define BOOKING_SERVICE_H

#include <string>
#include "AppointmentRepository.h"
#include "ClientRepository.h"
#include "EmployeeRepository.h"
#include "ServiceRepository.h"
#include "FinanceService.h"

class BookingService {
private:
    AppointmentRepository& appointment_repository;
    ClientRepository& client_repository;
    EmployeeRepository& employee_repository;
    ServiceRepository& service_repository;
    FinanceService& finance_service;

    bool employee_has_required_specialization(const Employee& employee, const Service& service) const;
    bool is_employee_available(int employee_id, const std::string& appointment_date, const std::string& appointment_time) const;
    int calculate_bonus_points(double price) const;

public:
    BookingService(AppointmentRepository& appointment_repository,
                   ClientRepository& client_repository,
                   EmployeeRepository& employee_repository,
                   ServiceRepository& service_repository,
                   FinanceService& finance_service);

    void create_appointment(int client_id,
                            int employee_id,
                            int service_id,
                            const std::string& appointment_date,
                            const std::string& appointment_time);

    void print_appointments();
};

#endif