#include "Employee.h"
#include <sstream>

Employee::Employee()
    : id(0), experience(0), rating(0.0), salaryBase(0.0), salaryPercent(0.0), status("active"), isDeleted(false) {}

Employee::Employee(int id,
                   const std::string& fullName,
                   const std::string& specialization,
                   int experience,
                   double rating,
                   double salaryBase,
                   double salaryPercent,
                   const std::string& status,
                   bool isDeleted)
    : id(id),
      fullName(fullName),
      specialization(specialization),
      experience(experience),
      rating(rating),
      salaryBase(salaryBase),
      salaryPercent(salaryPercent),
      status(status),
      isDeleted(isDeleted) {}

int Employee::getId() const {
    return id;
}

void Employee::setId(int id) {
    this->id = id;
}

std::string Employee::getFullName() const {
    return fullName;
}

void Employee::setFullName(const std::string& fullName) {
    this->fullName = fullName;
}

std::string Employee::getSpecialization() const {
    return specialization;
}

void Employee::setSpecialization(const std::string& specialization) {
    this->specialization = specialization;
}

int Employee::getExperience() const {
    return experience;
}

void Employee::setExperience(int experience) {
    this->experience = experience;
}

double Employee::getRating() const {
    return rating;
}

void Employee::setRating(double rating) {
    this->rating = rating;
}

double Employee::getSalaryBase() const {
    return salaryBase;
}

void Employee::setSalaryBase(double salaryBase) {
    this->salaryBase = salaryBase;
}

double Employee::getSalaryPercent() const {
    return salaryPercent;
}

void Employee::setSalaryPercent(double salaryPercent) {
    this->salaryPercent = salaryPercent;
}

std::string Employee::getStatus() const {
    return status;
}

void Employee::setStatus(const std::string& status) {
    this->status = status;
}

bool Employee::getIsDeleted() const {
    return isDeleted;
}

void Employee::setIsDeleted(bool isDeleted) {
    this->isDeleted = isDeleted;
}

std::string Employee::toString() const {
    std::ostringstream oss;
    oss << "Employee{id=" << id
        << ", fullName='" << fullName
        << "', specialization='" << specialization
        << "', experience=" << experience
        << ", rating=" << rating
        << ", salaryBase=" << salaryBase
        << ", salaryPercent=" << salaryPercent
        << ", status='" << status
        << "', isDeleted=" << (isDeleted ? "true" : "false")
        << "}";
    return oss.str();
}