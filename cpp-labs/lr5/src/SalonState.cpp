#include "SalonState.h"
#include <sstream>

SalonState::SalonState()
    : id(0), cashBalance(0.0), totalRevenue(0.0), totalExpenses(0.0) {}

SalonState::SalonState(int id,
                       const std::string& currentSalonDate,
                       double cashBalance,
                       const std::string& lastMonthSalaryPaid,
                       double totalRevenue,
                       double totalExpenses)
    : id(id),
      currentSalonDate(currentSalonDate),
      cashBalance(cashBalance),
      lastMonthSalaryPaid(lastMonthSalaryPaid),
      totalRevenue(totalRevenue),
      totalExpenses(totalExpenses) {}

int SalonState::getId() const {
    return id;
}

void SalonState::setId(int id) {
    this->id = id;
}

std::string SalonState::getCurrentSalonDate() const {
    return currentSalonDate;
}

void SalonState::setCurrentSalonDate(const std::string& currentSalonDate) {
    this->currentSalonDate = currentSalonDate;
}

double SalonState::getCashBalance() const {
    return cashBalance;
}

void SalonState::setCashBalance(double cashBalance) {
    this->cashBalance = cashBalance;
}

std::string SalonState::getLastMonthSalaryPaid() const {
    return lastMonthSalaryPaid;
}

void SalonState::setLastMonthSalaryPaid(const std::string& lastMonthSalaryPaid) {
    this->lastMonthSalaryPaid = lastMonthSalaryPaid;
}

double SalonState::getTotalRevenue() const {
    return totalRevenue;
}

void SalonState::setTotalRevenue(double totalRevenue) {
    this->totalRevenue = totalRevenue;
}

double SalonState::getTotalExpenses() const {
    return totalExpenses;
}

void SalonState::setTotalExpenses(double totalExpenses) {
    this->totalExpenses = totalExpenses;
}

std::string SalonState::toString() const {
    std::ostringstream oss;
    oss << "SalonState{id=" << id
        << ", currentSalonDate='" << currentSalonDate
        << "', cashBalance=" << cashBalance
        << ", lastMonthSalaryPaid='" << lastMonthSalaryPaid
        << "', totalRevenue=" << totalRevenue
        << ", totalExpenses=" << totalExpenses
        << "}";
    return oss.str();
}