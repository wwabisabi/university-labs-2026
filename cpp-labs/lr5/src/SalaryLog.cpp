#include "SalaryLog.h"
#include <sstream>

SalaryLog::SalaryLog()
    : id(0), employeeId(0), totalServicesSum(0.0), salaryCalculated(0.0), paidStatus(false) {}

SalaryLog::SalaryLog(int id,
                     int employeeId,
                     const std::string& monthYear,
                     double totalServicesSum,
                     double salaryCalculated,
                     bool paidStatus)
    : id(id),
      employeeId(employeeId),
      monthYear(monthYear),
      totalServicesSum(totalServicesSum),
      salaryCalculated(salaryCalculated),
      paidStatus(paidStatus) {}

int SalaryLog::getId() const {
    return id;
}

void SalaryLog::setId(int id) {
    this->id = id;
}

int SalaryLog::getEmployeeId() const {
    return employeeId;
}

void SalaryLog::setEmployeeId(int employeeId) {
    this->employeeId = employeeId;
}

std::string SalaryLog::getMonthYear() const {
    return monthYear;
}

void SalaryLog::setMonthYear(const std::string& monthYear) {
    this->monthYear = monthYear;
}

double SalaryLog::getTotalServicesSum() const {
    return totalServicesSum;
}

void SalaryLog::setTotalServicesSum(double totalServicesSum) {
    this->totalServicesSum = totalServicesSum;
}

double SalaryLog::getSalaryCalculated() const {
    return salaryCalculated;
}

void SalaryLog::setSalaryCalculated(double salaryCalculated) {
    this->salaryCalculated = salaryCalculated;
}

bool SalaryLog::getPaidStatus() const {
    return paidStatus;
}

void SalaryLog::setPaidStatus(bool paidStatus) {
    this->paidStatus = paidStatus;
}

std::string SalaryLog::toString() const {
    std::ostringstream oss;
    oss << "SalaryLog{id=" << id
        << ", employeeId=" << employeeId
        << ", monthYear='" << monthYear
        << "', totalServicesSum=" << totalServicesSum
        << ", salaryCalculated=" << salaryCalculated
        << ", paidStatus=" << (paidStatus ? "true" : "false")
        << "}";
    return oss.str();
}