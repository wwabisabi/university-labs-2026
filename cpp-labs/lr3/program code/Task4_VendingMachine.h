#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <iomanip>

class Product {
public:
    virtual ~Product() = default;
    virtual std::string getType() const = 0;
    
    bool isExpired(time_t currentTime) const;
    std::string getName() const { return name_; }
    double getPrice() const { return price_; }
    int getQuantity() const { return quantity_; }
    int getDaysUntilExpiry(time_t currentTime) const;
    void setExpiryDate(time_t expiry) { expiryDate_ = expiry; }
    time_t getExpiryDate() const { return expiryDate_; }
    
    void decreaseQuantity() { 
        if (quantity_ > 0) {
            quantity_--;
            sold_++;
            revenue_ += price_;
        }
    }
    void increaseQuantity(int q) { quantity_ += q; }
    int getSold() const { return sold_; }
    double getRevenue() const { return revenue_; }
    
protected:
    std::string name_;
    double price_;
    int quantity_;
    time_t expiryDate_;
    int sold_;
    double revenue_;
};

class ColdDrink : public Product {
public:
    ColdDrink(const std::string& name, double price, int quantity, int daysUntilExpiry);
    std::string getType() const override;
};

class HotDrink : public Product {
public:
    HotDrink(const std::string& name, double price, int quantity, int daysUntilExpiry);
    std::string getType() const override;
    bool needsHeating() const;
    void heatUp();
    void coolDown();
    bool isHot() const { return hot_; }
private:
    bool hot_;
    int coolingTimer_;
};

class Snack : public Product {
public:
    Snack(const std::string& name, double price, int quantity, int daysUntilExpiry);
    std::string getType() const override;
};

class Perishable : public Product {
public:
    Perishable(const std::string& name, double price, int quantity, int daysUntilExpiry);
    std::string getType() const override;
};

class MaintenanceMode {
public:
    MaintenanceMode();
    ~MaintenanceMode();
    static bool isActive() { return active_; }
private:
    static bool active_;
};

class VendingMachine {
public:
    VendingMachine();
    void run();
    
private:
    std::vector<std::unique_ptr<Product>> products_;
    double balance_;
    int totalSales_;
    double totalRevenue_;
    time_t simulatedTime_;
    int actionsCount_;
    
    void advanceSimulation();
    void showSimulationStatus() const;
    void showProducts() const;
    void insertMoney();
    void selectProduct();
    std::vector<int> calculateChange(int amount);
    void removeExpired();
    void checkFreshness() const;
    void checkTemperature();
    void showInventory() const;
    void showStats() const;
    void maintenanceMenu();
    void addProduct();
    void removeProduct();
    void refillAll();
    void refillPerishable(Perishable& perishable);
};

void VendingMachineTask();