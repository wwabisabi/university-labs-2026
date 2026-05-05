#include "Task1_Carsharing.h"
#include <iostream>
#include <algorithm>

// ==================== RentalSession RAII ====================

RentalSession::RentalSession(int carId, double rate, int timeLimitMinutes)
    : carId_(carId), baseRate_(rate), timeLimitMinutes_(timeLimitMinutes),
      active_(true), limitExceeded_(false), penaltyAmount_(0.0), actualMinutes_(0) {
    std::cout << "[RAII КОНСТРУКТОР] Аренда автомобиля #" << carId_ << " начата.\n";
    std::cout << "[RAII КОНСТРУКТОР] Лимит времени: " << timeLimitMinutes_ << " мин.\n";
    std::cout << "[RAII КОНСТРУКТОР] Базовая ставка: " << baseRate_ << " руб/мин.\n";
}

RentalSession::~RentalSession() {
    std::cout << "[RAII ДЕСТРУКТОР] Автоматическое завершение аренды автомобиля #" << carId_ << ".\n";
    
    if (limitExceeded_) {
        std::cout << "[RAII ДЕСТРУКТОР] Зафиксировано превышение лимита времени!\n";
        std::cout << "[RAII ДЕСТРУКТОР] Лимит: " << timeLimitMinutes_ << " мин.\n";
        std::cout << "[RAII ДЕСТРУКТОР] Фактически: " << actualMinutes_ << " мин.\n";
        std::cout << "[RAII ДЕСТРУКТОР] Превышение: " << (actualMinutes_ - timeLimitMinutes_) << " мин.\n";
        std::cout << "[RAII ДЕСТРУКТОР] Штраф: " << penaltyAmount_ << " руб.\n";
    } else {
        std::cout << "[RAII ДЕСТРУКТОР] Аренда завершена в пределах лимита.\n";
        std::cout << "[RAII ДЕСТРУКТОР] Заказано: " << timeLimitMinutes_ << " мин, использовано: " << actualMinutes_ << " мин.\n";
    }
    
    active_ = false;
}

double RentalSession::checkOverdue(int actualMinutes) {
    actualMinutes_ = actualMinutes;
    
    if (actualMinutes > timeLimitMinutes_) {
        limitExceeded_ = true;
        int overMinutes = actualMinutes - timeLimitMinutes_;
        penaltyAmount_ = baseRate_ * 2.0 * overMinutes;
        
        std::cout << "[ПРОВЕРКА] Превышение лимита на " << overMinutes << " мин!\n";
        std::cout << "[ПРОВЕРКА] Штраф: " << overMinutes << " * " << baseRate_ << " * 2 = " << penaltyAmount_ << " руб.\n";
        
        return penaltyAmount_;
    } else {
        std::cout << "[ПРОВЕРКА] Лимит не превышен. Осталось: " << (timeLimitMinutes_ - actualMinutes) << " мин.\n";
        return 0.0;
    }
}

// ==================== Car implementations ====================

void Car::addRentalRecord(const std::string& record) {
    history_.push_back(record);
}

void Car::showHistory() const {
    std::cout << "\n=== ИСТОРИЯ АРЕНД автомобиля #" << id_ << " (" << getType() << ") ===\n";
    if (history_.empty()) {
        std::cout << "  (пусто)\n";
    } else {
        for (size_t i = 0; i < history_.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << history_[i] << "\n";
        }
    }
    std::cout << "Всего аренд: " << history_.size() << "\n";
}

EconomyCar::EconomyCar(int id) { id_ = id; }
double EconomyCar::getBaseRate() const { return 8.0; }
std::string EconomyCar::getType() const { return "Эконом"; }
int EconomyCar::getMinAge() const { return 21; }
int EconomyCar::getMinExperience() const { return 2; }

BusinessCar::BusinessCar(int id) { id_ = id; }
double BusinessCar::getBaseRate() const { return 20.0; }
std::string BusinessCar::getType() const { return "Бизнес"; }
int BusinessCar::getMinAge() const { return 25; }
int BusinessCar::getMinExperience() const { return 5; }

Minivan::Minivan(int id) { id_ = id; }
double Minivan::getBaseRate() const { return 15.0; }
std::string Minivan::getType() const { return "Минивэн"; }
int Minivan::getMinAge() const { return 23; }
int Minivan::getMinExperience() const { return 3; }

ElectricCar::ElectricCar(int id) { id_ = id; }
double ElectricCar::getBaseRate() const { return 10.0; }
std::string ElectricCar::getType() const { return "Электромобиль"; }
int ElectricCar::getMinAge() const { return 21; }
int ElectricCar::getMinExperience() const { return 2; }

// ==================== Tariffs ====================

double PerMinuteTariff::calculateCost(double baseRate, int minutes, bool isNight) const {
    double modifier = isNight ? 1.2 : 1.0;
    double cost = baseRate * minutes * modifier;
    return cost;
}
std::string PerMinuteTariff::getName() const { return "Поминутный"; }

double DailyTariff::calculateCost(double baseRate, int minutes, bool isNight) const {
    int days = (minutes + 1439) / 1440;
    double dayRate = baseRate * 200;
    double modifier = isNight ? 1.2 : 1.0;
    double cost = dayRate * days * modifier;
    return cost;
}
std::string DailyTariff::getName() const { return "Посуточный"; }

double PackageTariff::calculateCost(double baseRate, int minutes, bool isNight) const {
    int packages = (minutes + 179) / 180;
    double packageRate = baseRate * 100;
    double modifier = isNight ? 1.2 : 1.0;
    double cost = packageRate * packages * modifier;
    return cost;
}
std::string PackageTariff::getName() const { return "Пакетный (3 часа)"; }

// ==================== Customer ====================

Customer::Customer(const std::string& name)
    : name_(name), totalRentals_(0), totalSpent_(0) {}

void Customer::addRental(const std::string& carType, double cost) {
    totalRentals_++;
    totalSpent_ += cost;
    rentalHistory_.push_back(carType + " - " + std::to_string(cost) + " руб.");
}

double Customer::calculateBonus(double cost) const {
    double discount = 0.0;
    
    if (totalRentals_ >= 10) {
        discount = 0.15;
    } else if (totalRentals_ >= 6) {
        discount = 0.10;
    } else if (totalRentals_ >= 3) {
        discount = 0.05;
    }
    
    if ((totalRentals_ + 1) % 3 == 0) {
        discount += 0.05;
    }
    
    return cost * discount;
}

void Customer::showStats() const {
    std::cout << "\n=== СТАТИСТИКА КЛИЕНТА: " << name_ << " ===\n";
    std::cout << "Всего аренд: " << totalRentals_ << "\n";
    std::cout << "Общая сумма: " << totalSpent_ << " руб.\n";
    
    std::string status;
    if (totalRentals_ >= 10) status = "Платиновый (скидка 15%)";
    else if (totalRentals_ >= 6) status = "Золотой (скидка 10%)";
    else if (totalRentals_ >= 3) status = "Серебряный (скидка 5%)";
    else status = "Новый клиент";
    
    std::cout << "Статус: " << status << "\n";
    
    if (!rentalHistory_.empty()) {
        std::cout << "История аренд клиента:\n";
        for (const auto& rec : rentalHistory_) {
            std::cout << "  - " << rec << "\n";
        }
    }
}

// ==================== CarsharingSystem ====================

CarsharingSystem::CarsharingSystem() : currentCustomer_(nullptr) {
    createCar(0, 1);
    createCar(1, 2);
    createCar(2, 3);
    createCar(3, 4);
}

void CarsharingSystem::createCar(int type, int id) {
    switch(type) {
        case 0: cars_.push_back(std::make_unique<EconomyCar>(id)); break;
        case 1: cars_.push_back(std::make_unique<BusinessCar>(id)); break;
        case 2: cars_.push_back(std::make_unique<Minivan>(id)); break;
        case 3: cars_.push_back(std::make_unique<ElectricCar>(id)); break;
    }
}

void CarsharingSystem::showCars() const {
    std::cout << "\n=== ДОСТУПНЫЕ АВТОМОБИЛИ ===\n";
    for (const auto& car : cars_) {
        std::cout << "  #" << car->getId() << " " << car->getType()
                  << " | Ставка: " << car->getBaseRate() << " руб/мин"
                  << " | Мин. возраст: " << car->getMinAge()
                  << " | Мин. стаж: " << car->getMinExperience()
                  << " | Аренд: " << car->getRentalCount() << "\n";
    }
}

bool CarsharingSystem::checkEligibility(const Car& car, int age, int experience) {
    if (age < car.getMinAge()) {
        std::cout << "ОТКАЗ: минимальный возраст для " << car.getType()
                  << " - " << car.getMinAge() << " лет (вам " << age << ").\n";
        return false;
    }
    if (experience < car.getMinExperience()) {
        std::cout << "ОТКАЗ: минимальный стаж для " << car.getType()
                  << " - " << car.getMinExperience() << " лет (у вас " << experience << ").\n";
        return false;
    }
    return true;
}

Customer* CarsharingSystem::findOrCreateCustomer(const std::string& name) {
    auto it = std::find_if(customers_.begin(), customers_.end(),
        [&name](const auto& c) { return c->getName() == name; });
    if (it != customers_.end()) {
        return it->get();
    }
    customers_.push_back(std::make_unique<Customer>(name));
    return customers_.back().get();
}

void CarsharingSystem::showCarHistory() const {
    std::cout << "Введите ID автомобиля: ";
    int carId;
    std::cin >> carId;
    auto it = std::find_if(cars_.begin(), cars_.end(),
        [carId](const auto& c) { return c->getId() == carId; });
    if (it != cars_.end()) {
        (*it)->showHistory();
    } else {
        std::cout << "Автомобиль не найден.\n";
    }
}

void CarsharingSystem::showCustomerStats() const {
    if (currentCustomer_) {
        currentCustomer_->showStats();
    } else {
        std::cout << "Сначала выполните аренду.\n";
    }
}

void CarsharingSystem::rentCar() {
    // Запрашиваем имя клиента
    std::cout << "Введите ваше имя: ";
    std::string name;
    std::cin >> name;
    currentCustomer_ = findOrCreateCustomer(name);
    
    if (currentCustomer_->getTotalRentals() > 0) {
        std::cout << "С возвращением, " << name << "!\n";
        currentCustomer_->showStats();
    } else {
        std::cout << "Добро пожаловать, " << name << "! Это ваша первая аренда.\n";
    }
    
    // Показываем автомобили
    showCars();
    std::cout << "Введите ID автомобиля: ";
    int carId;
    std::cin >> carId;
    
    auto it = std::find_if(cars_.begin(), cars_.end(),
        [carId](const auto& c) { return c->getId() == carId; });
    if (it == cars_.end()) throw std::runtime_error("Автомобиль не найден.");
    
    Car& car = **it;
    
    // Проверка возраста и стажа
    std::cout << "Ваш возраст: ";
    int age;
    std::cin >> age;
    
    std::cout << "Ваш стаж (лет): ";
    int experience;
    std::cin >> experience;
    
    if (!checkEligibility(car, age, experience)) return;
    
    // Выбор тарифа
    std::cout << "\nВыберите тариф:\n";
    std::cout << "1. Поминутный\n";
    std::cout << "2. Посуточный\n";
    std::cout << "3. Пакетный (3 часа)\n";
    int t;
    std::cin >> t;
    
    std::unique_ptr<ITariff> tariff;
    switch(t) {
        case 1: tariff = std::make_unique<PerMinuteTariff>(); break;
        case 2: tariff = std::make_unique<DailyTariff>(); break;
        case 3: tariff = std::make_unique<PackageTariff>(); break;
        default: throw std::runtime_error("Неверный тариф.");
    }
    
    // Лимит времени
    std::cout << "На сколько минут берете авто (лимит): ";
    int limitMinutes;
    std::cin >> limitMinutes;
    
    // Ночное время
    std::cout << "Ночное время? (1-да, 0-нет): ";
    int night;
    std::cin >> night;
    
    double finalCost = 0.0;
    int actualMinutes = 0;
    bool wasOverdue = false;
    int overMinutes = 0;
    double penaltyAmount = 0.0;
    double costForLimit = 0.0;
    
    // ================================================================
    // БЛОК RAII
    // Объект session существует только внутри этих фигурных скобок.
    // При выходе из блока (даже при исключении) вызовется деструктор.
    // ================================================================
    {
        RentalSession session(car.getId(), car.getBaseRate(), limitMinutes);
        
        // Имитация: запрашиваем фактическое время
        std::cout << "Сколько минут фактически прошло? ";
        std::cin >> actualMinutes;
        
        double penalty = session.checkOverdue(actualMinutes);
        
        if (penalty > 0) {
            // ======== ПРЕВЫШЕНИЕ ЛИМИТА ========
            wasOverdue = true;
            overMinutes = actualMinutes - limitMinutes;
            penaltyAmount = penalty;
            
            // Стоимость за лимитное время по обычному тарифу
            costForLimit = tariff->calculateCost(car.getBaseRate(), limitMinutes, night == 1);
            
            // Штраф за превышение: двойной тариф за каждую минуту превышения
            double costForOver = car.getBaseRate() * 2.0 * overMinutes;
            
            std::cout << "\n=== РАСЧЕТ СТОИМОСТИ (превышение лимита) ===\n";
            std::cout << "Лимит: " << limitMinutes << " мин, фактически: " << actualMinutes << " мин.\n";
            std::cout << "Превышение: " << overMinutes << " мин.\n";
            std::cout << "Стоимость за лимит (" << limitMinutes << " мин по тарифу \"" << tariff->getName() << "\"): " << costForLimit << " руб.\n";
            std::cout << "Штраф за превышение (" << overMinutes << " мин * " << car.getBaseRate() << " * 2): " << costForOver << " руб.\n";
            
            finalCost = costForLimit + costForOver;
        } else {
            // ======== БЕЗ ПРЕВЫШЕНИЯ ========
            // Платим только за фактическое время
            finalCost = tariff->calculateCost(car.getBaseRate(), actualMinutes, night == 1);
            
            std::cout << "\n=== РАСЧЕТ СТОИМОСТИ (без превышения) ===\n";
            std::cout << "Лимит: " << limitMinutes << " мин, фактически: " << actualMinutes << " мин.\n";
            std::cout << "Стоимость за фактическое время (" << actualMinutes << " мин по тарифу \"" << tariff->getName() << "\"): " << finalCost << " руб.\n";
        }
        
        // Бонусная система
        double bonus = currentCustomer_->calculateBonus(finalCost);
        if (bonus > 0) {
            std::cout << "Бонус постоянного клиента: -" << bonus << " руб.\n";
            finalCost -= bonus;
        }
        
        std::cout << "ИТОГО К ОПЛАТЕ: " << finalCost << " руб.\n";
        
        std::cout << "\n>>> Выход из блока RAII, вызов деструктора RentalSession... <<<\n";
    }
    // ================================================================
    // КОНЕЦ БЛОКА RAII
    // Деструктор RentalSession уже вызван, ресурсы освобождены
    // ================================================================
    
    // Формируем запись в историю автомобиля
    std::string record = "Клиент: " + currentCustomer_->getName() +
                        ", Тариф: " + tariff->getName() +
                        ", Лимит: " + std::to_string(limitMinutes) + " мин" +
                        ", Факт: " + std::to_string(actualMinutes) + " мин" +
                        (night ? ", ночь" : ", день");
    if (wasOverdue) {
        record += ", Превышение: " + std::to_string(overMinutes) + " мин";
        record += ", Штраф: " + std::to_string(penaltyAmount) + " руб";
    }
    record += ", Итого: " + std::to_string(finalCost) + " руб.";
    
    car.addRentalRecord(record);
    currentCustomer_->addRental(car.getType(), finalCost);
    
    std::cout << "\nАренда завершена.\n";
    car.showHistory();
}

void CarsharingSystem::run() {
    bool running = true;
    while (running) {
        std::cout << "\n=== КАРШЕРИНГ ===\n";
        std::cout << "1. Арендовать автомобиль\n";
        std::cout << "2. История аренд автомобиля\n";
        std::cout << "3. Статистика клиента\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";
        
        int choice;
        std::cin >> choice;
        
        try {
            switch (choice) {
                case 1: rentCar(); break;
                case 2: showCarHistory(); break;
                case 3: showCustomerStats(); break;
                case 0: running = false; break;
                default: std::cout << "Неверный выбор.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
    }
}

void CarSharingTask() {
    CarsharingSystem system;
    system.run();
}