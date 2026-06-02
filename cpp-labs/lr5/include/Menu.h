#ifndef MENU_H
#define MENU_H

#include "BookingService.h"
#include "FinanceService.h"
#include "SalaryService.h"
#include "EmployeeRepository.h"
#include "ClientRepository.h"
#include "ServiceRepository.h"
#include "AppointmentRepository.h"
#include "SalonStateRepository.h"
#include "SalaryLogRepository.h"

class Menu {
private:
    EmployeeRepository& employee_repository;
    ClientRepository& client_repository;
    ServiceRepository& service_repository;
    AppointmentRepository& appointment_repository;
    SalonStateRepository& salon_state_repository;
    SalaryLogRepository& salary_log_repository;

    BookingService& booking_service;
    FinanceService& finance_service;
    SalaryService& salary_service;

    void show_employees();
    void show_clients();
    void show_services();
    void show_appointments();
    void show_finance();
    void create_appointment();
    void calculate_salaries();

public:
    Menu(EmployeeRepository& employee_repository,
         ClientRepository& client_repository,
         ServiceRepository& service_repository,
         AppointmentRepository& appointment_repository,
         SalonStateRepository& salon_state_repository,
         SalaryLogRepository& salary_log_repository,
         BookingService& booking_service,
         FinanceService& finance_service,
         SalaryService& salary_service);

    void run();
};

#endif