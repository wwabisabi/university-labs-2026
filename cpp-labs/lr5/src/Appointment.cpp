#include "Appointment.h"
#include <sstream>

Appointment::Appointment()
    : id(0), clientId(0), employeeId(0), serviceId(0), priceAtBooking(0.0) {}

Appointment::Appointment(int id,
                         int clientId,
                         int employeeId,
                         int serviceId,
                         const std::string& appointmentDate,
                         const std::string& appointmentTime,
                         const std::string& status,
                         double priceAtBooking)
    : id(id),
      clientId(clientId),
      employeeId(employeeId),
      serviceId(serviceId),
      appointmentDate(appointmentDate),
      appointmentTime(appointmentTime),
      status(status),
      priceAtBooking(priceAtBooking) {}

int Appointment::getId() const {
    return id;
}

void Appointment::setId(int id) {
    this->id = id;
}

int Appointment::getClientId() const {
    return clientId;
}

void Appointment::setClientId(int clientId) {
    this->clientId = clientId;
}

int Appointment::getEmployeeId() const {
    return employeeId;
}

void Appointment::setEmployeeId(int employeeId) {
    this->employeeId = employeeId;
}

int Appointment::getServiceId() const {
    return serviceId;
}

void Appointment::setServiceId(int serviceId) {
    this->serviceId = serviceId;
}

std::string Appointment::getAppointmentDate() const {
    return appointmentDate;
}

void Appointment::setAppointmentDate(const std::string& appointmentDate) {
    this->appointmentDate = appointmentDate;
}

std::string Appointment::getAppointmentTime() const {
    return appointmentTime;
}

void Appointment::setAppointmentTime(const std::string& appointmentTime) {
    this->appointmentTime = appointmentTime;
}

std::string Appointment::getStatus() const {
    return status;
}

void Appointment::setStatus(const std::string& status) {
    this->status = status;
}

double Appointment::getPriceAtBooking() const {
    return priceAtBooking;
}

void Appointment::setPriceAtBooking(double priceAtBooking) {
    this->priceAtBooking = priceAtBooking;
}

std::string Appointment::toString() const {
    std::ostringstream oss;
    oss << "Appointment{id=" << id
        << ", clientId=" << clientId
        << ", employeeId=" << employeeId
        << ", serviceId=" << serviceId
        << ", appointmentDate='" << appointmentDate
        << "', appointmentTime='" << appointmentTime
        << "', status='" << status
        << "', priceAtBooking=" << priceAtBooking
        << "}";
    return oss.str();
}