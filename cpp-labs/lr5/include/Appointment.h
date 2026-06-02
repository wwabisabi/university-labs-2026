#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>

class Appointment {
private:
    int id;
    int clientId;
    int employeeId;
    int serviceId;
    std::string appointmentDate;
    std::string appointmentTime;
    std::string status;
    double priceAtBooking;

public:
    Appointment();
    Appointment(int id,
                int clientId,
                int employeeId,
                int serviceId,
                const std::string& appointmentDate,
                const std::string& appointmentTime,
                const std::string& status,
                double priceAtBooking);

    int getId() const;
    void setId(int id);

    int getClientId() const;
    void setClientId(int clientId);

    int getEmployeeId() const;
    void setEmployeeId(int employeeId);

    int getServiceId() const;
    void setServiceId(int serviceId);

    std::string getAppointmentDate() const;
    void setAppointmentDate(const std::string& appointmentDate);

    std::string getAppointmentTime() const;
    void setAppointmentTime(const std::string& appointmentTime);

    std::string getStatus() const;
    void setStatus(const std::string& status);

    double getPriceAtBooking() const;
    void setPriceAtBooking(double priceAtBooking);

    std::string toString() const;
};

#endif