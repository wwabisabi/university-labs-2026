#include "Client.h"
#include <sstream>

Client::Client()
    : id(0), totalSpent(0.0), visitsCount(0), bonusPoints(0), isDeleted(false) {}

Client::Client(int id,
               const std::string& fullName,
               const std::string& phone,
               const std::string& firstVisitDate,
               double totalSpent,
               int visitsCount,
               int bonusPoints,
               bool isDeleted)
    : id(id),
      fullName(fullName),
      phone(phone),
      firstVisitDate(firstVisitDate),
      totalSpent(totalSpent),
      visitsCount(visitsCount),
      bonusPoints(bonusPoints),
      isDeleted(isDeleted) {}

int Client::getId() const {
    return id;
}

void Client::setId(int id) {
    this->id = id;
}

std::string Client::getFullName() const {
    return fullName;
}

void Client::setFullName(const std::string& fullName) {
    this->fullName = fullName;
}

std::string Client::getPhone() const {
    return phone;
}

void Client::setPhone(const std::string& phone) {
    this->phone = phone;
}

std::string Client::getFirstVisitDate() const {
    return firstVisitDate;
}

void Client::setFirstVisitDate(const std::string& firstVisitDate) {
    this->firstVisitDate = firstVisitDate;
}

double Client::getTotalSpent() const {
    return totalSpent;
}

void Client::setTotalSpent(double totalSpent) {
    this->totalSpent = totalSpent;
}

int Client::getVisitsCount() const {
    return visitsCount;
}

void Client::setVisitsCount(int visitsCount) {
    this->visitsCount = visitsCount;
}

int Client::getBonusPoints() const {
    return bonusPoints;
}

void Client::setBonusPoints(int bonusPoints) {
    this->bonusPoints = bonusPoints;
}

bool Client::getIsDeleted() const {
    return isDeleted;
}

void Client::setIsDeleted(bool isDeleted) {
    this->isDeleted = isDeleted;
}

std::string Client::toString() const {
    std::ostringstream oss;
    oss << "Client{id=" << id
        << ", fullName='" << fullName
        << "', phone='" << phone
        << "', firstVisitDate='" << firstVisitDate
        << "', totalSpent=" << totalSpent
        << ", visitsCount=" << visitsCount
        << ", bonusPoints=" << bonusPoints
        << ", isDeleted=" << (isDeleted ? "true" : "false")
        << "}";
    return oss.str();
}