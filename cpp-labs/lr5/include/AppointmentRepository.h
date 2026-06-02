#ifndef APPOINTMENT_REPOSITORY_H
#define APPOINTMENT_REPOSITORY_H

#include <vector>
#include "Appointment.h"
#include "DatabaseManager.h"

class AppointmentRepository {
private:
    DatabaseManager& db;

public:
    explicit AppointmentRepository(DatabaseManager& db);

    std::vector<Appointment> getAll();
    Appointment getById(int id);
    void add(const Appointment& appointment);
    void update(const Appointment& appointment);
};

#endif