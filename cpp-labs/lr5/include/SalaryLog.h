#ifndef SALARY_LOG_H
#define SALARY_LOG_H

#include <string>

class SalaryLog {
private:
    int id;
    int employeeId;
    std::string monthYear;
    double totalServicesSum;
    double salaryCalculated;
    bool paidStatus;

public:
    SalaryLog();
    SalaryLog(int id,
              int employeeId,
              const std::string& monthYear,
              double totalServicesSum,
              double salaryCalculated,
              bool paidStatus);

    int getId() const;
    void setId(int id);

    int getEmployeeId() const;
    void setEmployeeId(int employeeId);

    std::string getMonthYear() const;
    void setMonthYear(const std::string& monthYear);

    double getTotalServicesSum() const;
    void setTotalServicesSum(double totalServicesSum);

    double getSalaryCalculated() const;
    void setSalaryCalculated(double salaryCalculated);

    bool getPaidStatus() const;
    void setPaidStatus(bool paidStatus);

    std::string toString() const;
};

#endif