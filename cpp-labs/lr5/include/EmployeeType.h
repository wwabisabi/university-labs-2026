#ifndef EMPLOYEE_TYPE_H
#define EMPLOYEE_TYPE_H

#include <string>

class EmployeeType {
private:
    int id;
    int employeeId;
    std::string type;
    int maxClientsPerDay;
    int avgServiceTime;

public:
    EmployeeType();
    EmployeeType(int id,
                 int employeeId,
                 const std::string& type,
                 int maxClientsPerDay,
                 int avgServiceTime);

    int getId() const;
    void setId(int id);

    int getEmployeeId() const;
    void setEmployeeId(int employeeId);

    std::string getType() const;
    void setType(const std::string& type);

    int getMaxClientsPerDay() const;
    void setMaxClientsPerDay(int maxClientsPerDay);

    int getAvgServiceTime() const;
    void setAvgServiceTime(int avgServiceTime);

    std::string toString() const;
};

#endif