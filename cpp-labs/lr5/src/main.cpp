#include <iostream>

#include "DatabaseManager.h"
#include "EmployeeRepository.h"
#include "ClientRepository.h"
#include "ServiceRepository.h"
#include "AppointmentRepository.h"
#include "SalonStateRepository.h"
#include "SalaryLogRepository.h"

#include "BookingService.h"
#include "FinanceService.h"
#include "SalaryService.h"
#include "Menu.h"

int main() {
    try {
        DatabaseManager& db = DatabaseManager::getInstance(
            "localhost",
            "5432",
            "beauty_salon_db",
            "postgres",
            "postgres"
        );

        EmployeeRepository employee_repository(db);
        ClientRepository client_repository(db);
        ServiceRepository service_repository(db);
        AppointmentRepository appointment_repository(db);
        SalonStateRepository salon_state_repository(db);
        SalaryLogRepository salary_log_repository(db);

        FinanceService finance_service(salon_state_repository);
        BookingService booking_service(
            appointment_repository,
            client_repository,
            employee_repository,
            service_repository,
            finance_service
        );
        SalaryService salary_service(
            employee_repository,
            salary_log_repository,
            appointment_repository,
            finance_service
        );

        Menu menu(
            employee_repository,
            client_repository,
            service_repository,
            appointment_repository,
            salon_state_repository,
            salary_log_repository,
            booking_service,
            finance_service,
            salary_service
        );

        menu.run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}