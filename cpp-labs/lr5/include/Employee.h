#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

class Employee {
private:
    int id;
    std::string fullName;
    std::string specialization;
    int experience;
    double rating;
    double salaryBase;
    double salaryPercent;
    std::string status;
    bool isDeleted;

public:
    Employee();
    Employee(int id,
             const std::string& fullName,
             const std::string& specialization,
             int experience,
             double rating,
             double salaryBase,
             double salaryPercent,
             const std::string& status,
             bool isDeleted);

    int getId() const;
    void setId(int id);

    std::string getFullName() const;
    void setFullName(const std::string& fullName);

    std::string getSpecialization() const;
    void setSpecialization(const std::string& specialization);

    int getExperience() const;
    void setExperience(int experience);

    double getRating() const;
    void setRating(double rating);

    double getSalaryBase() const;
    void setSalaryBase(double salaryBase);

    double getSalaryPercent() const;
    void setSalaryPercent(double salaryPercent);

    std::string getStatus() const;
    void setStatus(const std::string& status);

    bool getIsDeleted() const;
    void setIsDeleted(bool isDeleted);

    std::string toString() const;
};

#endif