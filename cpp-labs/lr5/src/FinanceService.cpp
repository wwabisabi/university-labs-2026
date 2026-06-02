#include "FinanceService.h"
#include "Exceptions.h"

#include <iostream>
#include <vector>

FinanceService::FinanceService(SalonStateRepository& salon_state_repository)
    : salon_state_repository(salon_state_repository) {}

SalonState FinanceService::get_current_state() {
    std::vector<SalonState> states = salon_state_repository.getAll();
    if (states.empty()) {
        throw NotFoundException("Salon state not found");
    }
    return states.front();
}

void FinanceService::add_revenue(double amount) {
    SalonState state = get_current_state();
    state.setCashBalance(state.getCashBalance() + amount);
    state.setTotalRevenue(state.getTotalRevenue() + amount);
    salon_state_repository.update(state);
}

void FinanceService::add_expense(double amount) {
    SalonState state = get_current_state();
    state.setCashBalance(state.getCashBalance() - amount);
    state.setTotalExpenses(state.getTotalExpenses() + amount);
    salon_state_repository.update(state);
}

void FinanceService::print_current_state() {
    SalonState state = get_current_state();
    std::cout << state.toString() << std::endl;
}