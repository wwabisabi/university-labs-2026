#include "AppointmentRepository.h"
#include "Exceptions.h"
#include <sstream>

AppointmentRepository::AppointmentRepository(DatabaseManager& db) : db(db) {}

std::vector<Appointment> AppointmentRepository::getAll() {
    std::vector<Appointment> appointments;

    PGresult* result = db.executeQuery(
        "SELECT id, client_id, employee_id, service_id, appointment_date, appointment_time, status, price_at_booking "
        "FROM appointments ORDER BY id;"
    );

    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        appointments.emplace_back(
            std::stoi(PQgetvalue(result, i, 0)),
            std::stoi(PQgetvalue(result, i, 1)),
            std::stoi(PQgetvalue(result, i, 2)),
            std::stoi(PQgetvalue(result, i, 3)),
            PQgetvalue(result, i, 4),
            PQgetvalue(result, i, 5),
            PQgetvalue(result, i, 6),
            std::stod(PQgetvalue(result, i, 7))
        );
    }

    PQclear(result);
    return appointments;
}

Appointment AppointmentRepository::getById(int id) {
    std::ostringstream query;
    query << "SELECT id, client_id, employee_id, service_id, appointment_date, appointment_time, status, price_at_booking "
          << "FROM appointments WHERE id = " << id << ";";

    PGresult* result = db.executeQuery(query.str());

    if (PQntuples(result) == 0) {
        PQclear(result);
        throw NotFoundException("Appointment not found");
    }

    Appointment appointment(
        std::stoi(PQgetvalue(result, 0, 0)),
        std::stoi(PQgetvalue(result, 0, 1)),
        std::stoi(PQgetvalue(result, 0, 2)),
        std::stoi(PQgetvalue(result, 0, 3)),
        PQgetvalue(result, 0, 4),
        PQgetvalue(result, 0, 5),
        PQgetvalue(result, 0, 6),
        std::stod(PQgetvalue(result, 0, 7))
    );

    PQclear(result);
    return appointment;
}

void AppointmentRepository::add(const Appointment& appointment) {
    std::ostringstream query;
    query << "INSERT INTO appointments (client_id, employee_id, service_id, appointment_date, appointment_time, status, price_at_booking) VALUES ("
          << appointment.getClientId() << ", "
          << appointment.getEmployeeId() << ", "
          << appointment.getServiceId() << ", '"
          << appointment.getAppointmentDate() << "', '"
          << appointment.getAppointmentTime() << "', '"
          << appointment.getStatus() << "', "
          << appointment.getPriceAtBooking() << ");";

    db.executeCommand(query.str());
}

void AppointmentRepository::update(const Appointment& appointment) {
    std::ostringstream query;
    query << "UPDATE appointments SET "
          << "client_id = " << appointment.getClientId() << ", "
          << "employee_id = " << appointment.getEmployeeId() << ", "
          << "service_id = " << appointment.getServiceId() << ", "
          << "appointment_date = '" << appointment.getAppointmentDate() << "', "
          << "appointment_time = '" << appointment.getAppointmentTime() << "', "
          << "status = '" << appointment.getStatus() << "', "
          << "price_at_booking = " << appointment.getPriceAtBooking() << " "
          << "WHERE id = " << appointment.getId() << ";";

    db.executeCommand(query.str());
}