#include "BookingService.h"
#include "Exceptions.h"

#include <iostream>
#include <vector>

BookingService::BookingService(AppointmentRepository& appointment_repository,
                               ClientRepository& client_repository,
                               EmployeeRepository& employee_repository,
                               ServiceRepository& service_repository,
                               FinanceService& finance_service)
    : appointment_repository(appointment_repository),
      client_repository(client_repository),
      employee_repository(employee_repository),
      service_repository(service_repository),
      finance_service(finance_service) {}

bool BookingService::employee_has_required_specialization(const Employee& employee, const Service& service) const {
    return employee.getSpecialization() == service.getRequiredSpecialization();
}

bool BookingService::is_employee_available(int employee_id,
                                           const std::string& appointment_date,
                                           const std::string& appointment_time) const {
    std::vector<Appointment> appointments = appointment_repository.getAll();

    for (const auto& appointment : appointments) {
        if (appointment.getEmployeeId() == employee_id &&
            appointment.getAppointmentDate() == appointment_date &&
            appointment.getAppointmentTime() == appointment_time &&
            appointment.getStatus() != "cancelled") {
            return false;
        }
    }

    return true;
}

int BookingService::calculate_bonus_points(double price) const {
    return static_cast<int>(price * 0.05);
}

void BookingService::create_appointment(int client_id,
                                        int employee_id,
                                        int service_id,
                                        const std::string& appointment_date,
                                        const std::string& appointment_time) {
    Client client = client_repository.getById(client_id);
    Employee employee = employee_repository.getById(employee_id);
    Service service = service_repository.getById(service_id);

    if (!employee_has_required_specialization(employee, service)) {
        throw InvalidInputException("Employee specialization does not match service");
    }

    if (!is_employee_available(employee_id, appointment_date, appointment_time)) {
        throw NoFreeSlotException("Employee is busy at this date and time");
    }

    Appointment appointment(
        0,
        client_id,
        employee_id,
        service_id,
        appointment_date,
        appointment_time,
        "scheduled",
        service.getPrice()
    );

    appointment_repository.add(appointment);

    client.setVisitsCount(client.getVisitsCount() + 1);
    client.setTotalSpent(client.getTotalSpent() + service.getPrice());
    client.setBonusPoints(client.getBonusPoints() + calculate_bonus_points(service.getPrice()));
    client_repository.update(client);

    finance_service.add_revenue(service.getPrice());
    finance_service.add_expense(service.getMaterialCost());
}

void BookingService::print_appointments() {
    std::vector<Appointment> appointments = appointment_repository.getAll();
    for (const auto& appointment : appointments) {
        std::cout << appointment.toString() << std::endl;
    }
}