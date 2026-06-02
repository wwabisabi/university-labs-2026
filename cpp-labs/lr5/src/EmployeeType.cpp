#include "EmployeeType.h"
#include <sstream>

EmployeeType::EmployeeType()
    : id(0), employeeId(0), maxClientsPerDay(0), avgServiceTime(0) {}

EmployeeType::EmployeeType(int id,
                           int employeeId,
                           const std::string& type,
                           int maxClientsPerDay,
                           int avgServiceTime)
    : id(id),
      employeeId(employeeId),
      type(type),
      maxClientsPerDay(maxClientsPerDay),
      avgServiceTime(avgServiceTime) {}

int EmployeeType::getId() const {
    return id;
}

void EmployeeType::setId(int id) {
    this->id = id;
}

int EmployeeType::getEmployeeId() const {
    return employeeId;
}

void EmployeeType::setEmployeeId(int employeeId) {
    this->employeeId = employeeId;
}

std::string EmployeeType::getType() const {
    return type;
}

void EmployeeType::setType(const std::string& type) {
    this->type = type;
}

int EmployeeType::getMaxClientsPerDay() const {
    return maxClientsPerDay;
}

void EmployeeType::setMaxClientsPerDay(int maxClientsPerDay) {
    this->maxClientsPerDay = maxClientsPerDay;
}

int EmployeeType::getAvgServiceTime() const {
    return avgServiceTime;
}

void EmployeeType::setAvgServiceTime(int avgServiceTime) {
    this->avgServiceTime = avgServiceTime;
}

std::string EmployeeType::toString() const {
    std::ostringstream oss;
    oss << "EmployeeType{id=" << id
        << ", employeeId=" << employeeId
        << ", type='" << type
        << "', maxClientsPerDay=" << maxClientsPerDay
        << ", avgServiceTime=" << avgServiceTime
        << "}";
    return oss.str();
}