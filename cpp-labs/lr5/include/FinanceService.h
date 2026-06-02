#ifndef FINANCE_SERVICE_H
#define FINANCE_SERVICE_H

#include "SalonStateRepository.h"

class FinanceService {
private:
    SalonStateRepository& salon_state_repository;

public:
    explicit FinanceService(SalonStateRepository& salon_state_repository);

    SalonState get_current_state();
    void add_revenue(double amount);
    void add_expense(double amount);
    void print_current_state();
};

#endif