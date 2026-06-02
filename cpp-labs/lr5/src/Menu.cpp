#include "Menu.h"
#include "Utils.h"

#include <iostream>
#include <vector>

Menu::Menu(EmployeeRepository& employee_repository,
           ClientRepository& client_repository,
           ServiceRepository& service_repository,
           AppointmentRepository& appointment_repository,
           SalonStateRepository& salon_state_repository,
           SalaryLogRepository& salary_log_repository,
           BookingService& booking_service,
           FinanceService& finance_service,
           SalaryService& salary_service)
    : employee_repository(employee_repository),
      client_repository(client_repository),
      service_repository(service_repository),
      appointment_repository(appointment_repository),
      salon_state_repository(salon_state_repository),
      salary_log_repository(salary_log_repository),
      booking_service(booking_service),
      finance_service(finance_service),
      salary_service(salary_service) {}

void Menu::show_employees() {
    utils::print_header("Employees");
    std::vector<Employee> employees = employee_repository.getAll();
    for (const auto& employee : employees) {
        std::cout << employee.toString() << std::endl;
    }
    utils::press_enter_to_continue();
}

void Menu::show_clients() {
    utils::print_header("Clients");
    std::vector<Client> clients = client_repository.getAll();
    for (const auto& client : clients) {
        std::cout << client.toString() << std::endl;
    }
    utils::press_enter_to_continue();
}

void Menu::show_services() {
    utils::print_header("Services");
    std::vector<Service> services = service_repository.getAll();
    for (const auto& service : services) {
        std::cout << service.toString() << std::endl;
    }
    utils::press_enter_to_continue();
}

void Menu::show_appointments() {
    utils::print_header("Appointments");
    booking_service.print_appointments();
    utils::press_enter_to_continue();
}

void Menu::show_finance() {
    utils::print_header("Salon finance");
    finance_service.print_current_state();
    utils::press_enter_to_continue();
}

void Menu::create_appointment() {
    utils::print_header("Create appointment");

    int client_id = utils::read_int("Enter client id: ");
    int employee_id = utils::read_int("Enter employee id: ");
    int service_id = utils::read_int("Enter service id: ");
    std::string appointment_date = utils::read_string("Enter date (YYYY-MM-DD): ");
    std::string appointment_time = utils::read_string("Enter time (HH:MM:SS): ");

    booking_service.create_appointment(
        client_id,
        employee_id,
        service_id,
        appointment_date,
        appointment_time
    );

    std::cout << "Appointment created successfully.\n";
    utils::press_enter_to_continue();
}

void Menu::calculate_salaries() {
    utils::print_header("Calculate salaries");

    if (utils::confirm("Calculate and save salaries for all employees?")) {
        salary_service.calculate_and_save_all_salaries();
        std::cout << "Salaries calculated and saved.\n";
    }

    utils::press_enter_to_continue();
}

void Menu::run() {
    while (true) {
        utils::print_header("Beauty salon management");

        std::cout << "1. Show employees\n";
        std::cout << "2. Show clients\n";
        std::cout << "3. Show services\n";
        std::cout << "4. Show appointments\n";
        std::cout << "5. Create appointment\n";
        std::cout << "6. Show finance\n";
        std::cout << "7. Calculate salaries\n";
        std::cout << "0. Exit\n";

        int choice = utils::read_int("Choose option: ");

        try {
            switch (choice) {
                case 1:
                    show_employees();
                    break;
                case 2:
                    show_clients();
                    break;
                case 3:
                    show_services();
                    break;
                case 4:
                    show_appointments();
                    break;
                case 5:
                    create_appointment();
                    break;
                case 6:
                    show_finance();
                    break;
                case 7:
                    calculate_salaries();
                    break;
                case 0:
                    return;
                default:
                    std::cout << "Unknown option.\n";
                    utils::press_enter_to_continue();
                    break;
            }
        } catch (const std::exception& ex) {
            std::cout << "Error: " << ex.what() << std::endl;
            utils::press_enter_to_continue();
        }
    }
}