#ifndef SALON_STATE_H
#define SALON_STATE_H

#include <string>

class SalonState {
private:
    int id;
    std::string currentSalonDate;
    double cashBalance;
    std::string lastMonthSalaryPaid;
    double totalRevenue;
    double totalExpenses;

public:
    SalonState();
    SalonState(int id,
               const std::string& currentSalonDate,
               double cashBalance,
               const std::string& lastMonthSalaryPaid,
               double totalRevenue,
               double totalExpenses);

    int getId() const;
    void setId(int id);

    std::string getCurrentSalonDate() const;
    void setCurrentSalonDate(const std::string& currentSalonDate);

    double getCashBalance() const;
    void setCashBalance(double cashBalance);

    std::string getLastMonthSalaryPaid() const;
    void setLastMonthSalaryPaid(const std::string& lastMonthSalaryPaid);

    double getTotalRevenue() const;
    void setTotalRevenue(double totalRevenue);

    double getTotalExpenses() const;
    void setTotalExpenses(double totalExpenses);

    std::string toString() const;
};

#endif