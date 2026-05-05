#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

// ==================== RAII: RentalSession ====================
// Ресурс "аренда" автоматически завершается при уничтожении объекта
class RentalSession {
public:
    RentalSession(int carId, double rate, int timeLimitMinutes);
    ~RentalSession();
    
    double checkOverdue(int actualMinutes);
    
    int getCarId() const { return carId_; }
    int getTimeLimit() const { return timeLimitMinutes_; }
    double getBaseRate() const { return baseRate_; }
    bool isLimitExceeded() const { return limitExceeded_; }
    double getPenalty() const { return penaltyAmount_; }
    
private:
    int carId_;
    double baseRate_;
    int timeLimitMinutes_;
    bool active_;
    bool limitExceeded_;
    double penaltyAmount_;
    int actualMinutes_;
};

// ==================== Автомобили (полиморфизм) ====================
class Car {
public:
    virtual ~Car() = default;
    virtual double getBaseRate() const = 0;
    virtual std::string getType() const = 0;
    virtual int getMinAge() const = 0;
    virtual int getMinExperience() const = 0;
    
    int getId() const { return id_; }
    void addRentalRecord(const std::string& record);
    void showHistory() const;
    int getRentalCount() const { return static_cast<int>(history_.size()); }    
protected:
    int id_;
    std::vector<std::string> history_;
};

class EconomyCar : public Car {
public:
    EconomyCar(int id);
    double getBaseRate() const override;
    std::string getType() const override;
    int getMinAge() const override;
    int getMinExperience() const override;
};

class BusinessCar : public Car {
public:
    BusinessCar(int id);
    double getBaseRate() const override;
    std::string getType() const override;
    int getMinAge() const override;
    int getMinExperience() const override;
};

class Minivan : public Car {
public:
    Minivan(int id);
    double getBaseRate() const override;
    std::string getType() const override;
    int getMinAge() const override;
    int getMinExperience() const override;
};

class ElectricCar : public Car {
public:
    ElectricCar(int id);
    double getBaseRate() const override;
    std::string getType() const override;
    int getMinAge() const override;
    int getMinExperience() const override;
};

// ==================== Тарифы (полиморфизм) ====================
class ITariff {
public:
    virtual ~ITariff() = default;
    virtual double calculateCost(double baseRate, int minutes, bool isNight) const = 0;
    virtual std::string getName() const = 0;
};

class PerMinuteTariff : public ITariff {
public:
    double calculateCost(double baseRate, int minutes, bool isNight) const override;
    std::string getName() const override;
};

class DailyTariff : public ITariff {
public:
    double calculateCost(double baseRate, int minutes, bool isNight) const override;
    std::string getName() const override;
};

class PackageTariff : public ITariff {
public:
    double calculateCost(double baseRate, int minutes, bool isNight) const override;
    std::string getName() const override;
};

// ==================== Клиент с бонусной системой ====================
class Customer {
public:
    Customer(const std::string& name);
    
    std::string getName() const { return name_; }
    int getTotalRentals() const { return totalRentals_; }
    double getTotalSpent() const { return totalSpent_; }
    
    void addRental(const std::string& carType, double cost);
    double calculateBonus(double cost) const;
    void showStats() const;
    
private:
    std::string name_;
    int totalRentals_;
    double totalSpent_;
    std::vector<std::string> rentalHistory_;
};

// ==================== Система каршеринга ====================
class CarsharingSystem {
public:
    CarsharingSystem();
    void run();
    
private:
    std::vector<std::unique_ptr<Car>> cars_;
    std::vector<std::unique_ptr<Customer>> customers_;
    Customer* currentCustomer_;
    
    void createCar(int type, int id);
    void showCars() const;
    void rentCar();
    void showCarHistory() const;
    void showCustomerStats() const;
    bool checkEligibility(const Car& car, int age, int experience);
    Customer* findOrCreateCustomer(const std::string& name);
};

void CarSharingTask();