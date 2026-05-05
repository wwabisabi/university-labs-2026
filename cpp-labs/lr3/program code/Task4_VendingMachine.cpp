#include "Task4_VendingMachine.h"
#include <iostream>
#include <iomanip>

// ==================== Product ====================
bool Product::isExpired(time_t currentTime) const {
    return currentTime > expiryDate_;
}

int Product::getDaysUntilExpiry(time_t currentTime) const {
    double diff = difftime(expiryDate_, currentTime);
    return static_cast<int>(diff / 86400);
}

// ==================== ColdDrink ====================
ColdDrink::ColdDrink(const std::string& name, double price, int quantity, int daysUntilExpiry) {
    name_ = name;
    price_ = price;
    quantity_ = quantity;
    expiryDate_ = time(nullptr) + daysUntilExpiry * 86400;
    sold_ = 0;
    revenue_ = 0;
}
std::string ColdDrink::getType() const { return "Холодный напиток"; }

// ==================== HotDrink ====================
HotDrink::HotDrink(const std::string& name, double price, int quantity, int daysUntilExpiry) {
    name_ = name;
    price_ = price;
    quantity_ = quantity;
    expiryDate_ = time(nullptr) + daysUntilExpiry * 86400;
    sold_ = 0;
    revenue_ = 0;
    hot_ = true;
    coolingTimer_ = 0;
}
std::string HotDrink::getType() const { return "Горячий напиток"; }

bool HotDrink::needsHeating() const {
    return !hot_;
}

void HotDrink::heatUp() {
    hot_ = true;
    coolingTimer_ = 0;
    std::cout << "  [OK] \"" << name_ << "\" подогрет до 85°C.\n";
}

void HotDrink::coolDown() {
    coolingTimer_++;
    if (coolingTimer_ >= 2) {
        hot_ = false;
    }
}

// ==================== Snack ====================
Snack::Snack(const std::string& name, double price, int quantity, int daysUntilExpiry) {
    name_ = name;
    price_ = price;
    quantity_ = quantity;
    expiryDate_ = time(nullptr) + daysUntilExpiry * 86400;
    sold_ = 0;
    revenue_ = 0;
}
std::string Snack::getType() const { return "Снек"; }

// ==================== Perishable ====================
Perishable::Perishable(const std::string& name, double price, int quantity, int daysUntilExpiry) {
    name_ = name;
    price_ = price;
    quantity_ = quantity;
    expiryDate_ = time(nullptr) + daysUntilExpiry * 86400;
    sold_ = 0;
    revenue_ = 0;
}
std::string Perishable::getType() const { return "Скоропортящийся"; }

// ==================== MaintenanceMode RAII ====================
bool MaintenanceMode::active_ = false;

MaintenanceMode::MaintenanceMode() {
    active_ = true;
    std::cout << "[RAII] Режим техобслуживания включен.\n";
}

MaintenanceMode::~MaintenanceMode() {
    active_ = false;
    std::cout << "[RAII] Режим техобслуживания выключен. Автомат готов к работе.\n";
}

// ==================== VendingMachine ====================
VendingMachine::VendingMachine() : balance_(0), totalSales_(0), totalRevenue_(0), actionsCount_(0) {
    simulatedTime_ = time(nullptr);
    
    auto cola = std::make_unique<ColdDrink>("Cola", 50, 10, 180);
    cola->setExpiryDate(simulatedTime_ + 180 * 86400);
    products_.push_back(std::move(cola));
    
    auto water = std::make_unique<ColdDrink>("Water", 30, 15, 365);
    water->setExpiryDate(simulatedTime_ + 365 * 86400);
    products_.push_back(std::move(water));
    
    auto coffee = std::make_unique<HotDrink>("Coffee", 70, 5, 90);
    coffee->setExpiryDate(simulatedTime_ + 90 * 86400);
    products_.push_back(std::move(coffee));
    
    auto tea = std::make_unique<HotDrink>("Tea", 50, 8, 90);
    tea->setExpiryDate(simulatedTime_ + 90 * 86400);
    products_.push_back(std::move(tea));
    
    auto chips = std::make_unique<Snack>("Chips", 40, 10, 120);
    chips->setExpiryDate(simulatedTime_ + 120 * 86400);
    products_.push_back(std::move(chips));
    
    auto chocolate = std::make_unique<Snack>("Chocolate", 60, 7, 180);
    chocolate->setExpiryDate(simulatedTime_ + 180 * 86400);
    products_.push_back(std::move(chocolate));
    
    auto sandwich = std::make_unique<Perishable>("Sandwich", 80, 4, 2);
    sandwich->setExpiryDate(simulatedTime_ + 2 * 86400);
    products_.push_back(std::move(sandwich));
    
    auto salad = std::make_unique<Perishable>("Salad", 100, 3, 3);
    salad->setExpiryDate(simulatedTime_ + 3 * 86400);
    products_.push_back(std::move(salad));
}

void VendingMachine::advanceSimulation() {
    actionsCount_++;
    
    if (actionsCount_ % 2 == 0) {
        simulatedTime_ += 12 * 3600;
        
        std::cout << "\n========================================\n";
        std::cout << "[СИМУЛЯЦИЯ] Прошло 12 часов.\n";
        
        bool somethingChanged = false;
        
        for (auto& p : products_) {
            if (p->getType() == "Горячий напиток") {
                HotDrink* hd = dynamic_cast<HotDrink*>(p.get());
                bool wasHot = hd->isHot();
                hd->coolDown();
                if (wasHot && !hd->isHot() && p->getQuantity() > 0) {
                    std::cout << "[!] \"" << p->getName() << "\" остыл.\n";
                    somethingChanged = true;
                }
            }
            if (p->getType() == "Скоропортящийся" && p->getQuantity() > 0) {
                int daysLeft = p->getDaysUntilExpiry(simulatedTime_);
                if (p->isExpired(simulatedTime_)) {
                    std::cout << "[!] ПРОСРОЧЕН! \"" << p->getName() << "\" (" << p->getQuantity() << " шт.)\n";
                    somethingChanged = true;
                } else if (daysLeft == 0) {
                    std::cout << "[!] \"" << p->getName() << "\" истекает сегодня!\n";
                    somethingChanged = true;
                } else if (daysLeft == 1) {
                    std::cout << "[!] \"" << p->getName() << "\" истекает завтра (остался 1 дн.)\n";
                    somethingChanged = true;
                }
            }
        }
        
        if (!somethingChanged) {
            std::cout << "[OK] Состояние товаров без изменений.\n";
        }
        std::cout << "========================================\n";
    }
}

void VendingMachine::showSimulationStatus() const {
    std::cout << "\n[СИМУЛЯЦИЯ] Текущее время: " << ctime(&simulatedTime_);
    std::cout << "[СИМУЛЯЦИЯ] Выполнено действий: " << actionsCount_ << "\n";
    std::cout << "[СИМУЛЯЦИЯ] Каждые 2 действия = +12 часов\n";
}

void VendingMachine::showProducts() const {
    std::cout << "\n=== ТОВАРЫ ===\n";
    for (size_t i = 0; i < products_.size(); ++i) {
        const auto& p = products_[i];
        std::cout << (i + 1) << ". " << p->getName() << " (" << p->getType() << ") - " << p->getPrice() << " руб.";
        
        if (p->getQuantity() == 0) {
            std::cout << " [НЕТ В НАЛИЧИИ]";
        } else {
            std::cout << " [" << p->getQuantity() << " шт.]";
        }
        
        if (p->isExpired(simulatedTime_) && p->getQuantity() > 0) {
            std::cout << " *ПРОСРОЧЕН*";
        }
        
        if (!p->isExpired(simulatedTime_)) {
            int daysLeft = p->getDaysUntilExpiry(simulatedTime_);
            if (p->getType() == "Скоропортящийся" && daysLeft <= 2 && p->getQuantity() > 0) {
                std::cout << " (годен ещё " << daysLeft << " дн.)";
            }
            if (p->getType() == "Горячий напиток" && p->getQuantity() > 0) {
                const HotDrink* hd = dynamic_cast<const HotDrink*>(p.get());
                if (!hd->isHot()) {
                    std::cout << " *ОСТЫЛ*";
                }
            }
        }
        std::cout << "\n";
    }
}

void VendingMachine::insertMoney() {
    std::cout << "Принимаются монеты: 1, 2, 5, 10 руб. (0 - завершить)\n";
    int coin;
    while (true) {
        std::cout << "Монета: ";
        std::cin >> coin;
        if (coin == 0) break;
        if (coin == 1 || coin == 2 || coin == 5 || coin == 10) {
            balance_ += coin;
            std::cout << "Баланс: " << balance_ << " руб.\n";
        } else {
            std::cout << "Монета не принимается.\n";
        }
    }
}

std::vector<int> VendingMachine::calculateChange(int amount) {
    std::vector<int> coins = {10, 5, 2, 1};
    std::vector<int> change;
    for (int coin : coins) {
        while (amount >= coin) {
            change.push_back(coin);
            amount -= coin;
        }
    }
    return change;
}

void VendingMachine::removeExpired() {
    bool removed = false;
    for (auto& p : products_) {
        if (p->isExpired(simulatedTime_) && p->getQuantity() > 0) {
            int qty = p->getQuantity();
            std::cout << "\n[!] \"" << p->getName() << "\" просрочен! Удалено " << qty << " шт.\n";
            p->increaseQuantity(-qty);
            removed = true;
        }
    }
    if (!removed) {
        std::cout << "[OK] Просроченных товаров нет.\n";
    }
}

void VendingMachine::selectProduct() {
    removeExpired();
    showProducts();
    
    std::cout << "Выберите номер товара (0 - отмена): ";
    int choice;
    std::cin >> choice;
    
    if (choice == 0) return;
    if (choice < 1 || choice > static_cast<int>(products_.size())) {
        throw std::runtime_error("Неверный номер товара.");
    }
    
    Product& product = *products_[static_cast<size_t>(choice - 1)];
    
    if (product.isExpired(simulatedTime_)) {
        throw std::runtime_error("Товар просрочен.");
    }
    if (product.getQuantity() == 0) {
        throw std::runtime_error("Товара нет в наличии.");
    }
    
    if (product.getType() == "Горячий напиток") {
        HotDrink& hotDrink = dynamic_cast<HotDrink&>(product);
        if (!hotDrink.isHot()) {
            std::cout << "  [!] Напиток остыл. Подогрев...\n";
            hotDrink.heatUp();
        } else {
            std::cout << "  [OK] Горячий, 85°C.\n";
        }
    }
    
    std::cout << "Цена: " << product.getPrice() << " руб.\n";
    insertMoney();
    
    if (balance_ < product.getPrice()) {
        std::cout << "Недостаточно средств. Возврат: " << balance_ << " руб.\n";
        balance_ = 0;
        return;
    }
    
    int changeAmount = static_cast<int>(balance_ - product.getPrice());
    product.decreaseQuantity();
    totalSales_++;
    totalRevenue_ += product.getPrice();
    
    std::cout << "Выдан: " << product.getName() << "\n";
    
    if (changeAmount > 0) {
        auto change = calculateChange(changeAmount);
        std::cout << "Сдача: ";
        for (size_t i = 0; i < change.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << change[i];
        }
        std::cout << " руб.\n";
    }
    
    balance_ = 0;
}

void VendingMachine::checkFreshness() const {
    std::cout << "\n=== ПРОВЕРКА СВЕЖЕСТИ ===\n";
    
    bool foundExpired = false;
    
    for (const auto& p : products_) {
        int daysLeft = p->getDaysUntilExpiry(simulatedTime_);
        
        std::cout << p->getName() << " (" << p->getType() << "): ";
        
        if (p->isExpired(simulatedTime_)) {
            std::cout << "ПРОСРОЧЕН (" << (-daysLeft) << " дн. назад)";
            foundExpired = true;
        } else if (p->getType() == "Скоропортящийся" && daysLeft <= 1) {
            std::cout << "Истекает (осталось " << daysLeft << " дн.)";
        } else {
            std::cout << "Свежий (" << daysLeft << " дн.)";
        }
        
        if (p->getQuantity() > 0) {
            std::cout << " - " << p->getQuantity() << " шт.";
        } else {
            std::cout << " - нет в наличии";
        }
        std::cout << "\n";
    }
    
    if (foundExpired) {
        std::cout << "\n[!] Есть просроченные товары. Они будут удалены при покупке.\n";
    } else {
        std::cout << "\n[OK] Просроченных товаров нет.\n";
    }
}

void VendingMachine::checkTemperature() {
    std::cout << "\n=== ТЕМПЕРАТУРА НАПИТКОВ ===\n";
    
    bool found = false;
    for (auto& p : products_) {
        if (p->getType() == "Горячий напиток") {
            found = true;
            HotDrink& hd = dynamic_cast<HotDrink&>(*p);
            
            std::cout << p->getName() << " (" << p->getQuantity() << " шт.): ";
            if (!hd.isHot()) {
                std::cout << "ОСТЫЛ\n  Подогреть? (1-да, 0-нет): ";
                int ans; std::cin >> ans;
                if (ans == 1) hd.heatUp();
            } else {
                std::cout << "ГОРЯЧИЙ (85°C)\n";
            }
        }
    }
    if (!found) std::cout << "Горячих напитков нет.\n";
}

void VendingMachine::showInventory() const {
    std::cout << "\n=== ИНВЕНТАРИЗАЦИЯ ===\n";
    std::cout << std::left << std::setw(15) << "Товар"
              << std::setw(18) << "Тип"
              << std::setw(8) << "Цена"
              << std::setw(10) << "Остаток"
              << std::setw(10) << "Продано"
              << std::setw(10) << "Выручка"
              << "Срок\n";
    std::cout << std::string(75, '-') << "\n";
    
    for (const auto& p : products_) {
        std::cout << std::left << std::setw(15) << p->getName()
                  << std::setw(18) << p->getType()
                  << std::setw(8) << p->getPrice()
                  << std::setw(10) << p->getQuantity()
                  << std::setw(10) << p->getSold()
                  << std::setw(10) << p->getRevenue();
        
        if (p->isExpired(simulatedTime_)) {
            std::cout << "ПРОСРОЧЕН";
        } else {
            std::cout << p->getDaysUntilExpiry(simulatedTime_) << " дн.";
        }
        std::cout << "\n";
    }
    std::cout << std::string(75, '-') << "\n";
    std::cout << "Продаж: " << totalSales_ << ", выручка: " << totalRevenue_ << " руб.\n";
}

void VendingMachine::showStats() const {
    std::cout << "\n=== СТАТИСТИКА ПРОДАЖ ===\n";
    std::cout << "Всего продано: " << totalSales_ << " шт.\n";
    std::cout << "Общая выручка: " << totalRevenue_ << " руб.\n";
    
    if (totalSales_ > 0) {
        std::cout << "\nПо товарам:\n";
        for (const auto& p : products_) {
            if (p->getSold() > 0) {
                std::cout << "  " << p->getName() << ": " << p->getSold() << " шт., " << p->getRevenue() << " руб.\n";
            }
        }
    }
}

void VendingMachine::refillPerishable(Perishable& perishable) {
    bool isExpired = perishable.isExpired(simulatedTime_);
    int qty = perishable.getQuantity();
    int daysLeft = perishable.getDaysUntilExpiry(simulatedTime_);
    
    // Случай 1: просрочен и есть на складе
    if (isExpired && qty > 0) {
        std::cout << "  Статус: ПРОСРОЧЕН (на " << (-daysLeft) << " дн.), на складе " << qty << " шт.\n";
        std::cout << "  [!] Сначала нужно списать просроченную партию.\n";
        std::cout << "  Списать просроченный товар? (1-да, 0-нет): ";
        int ans; std::cin >> ans;
        if (ans == 1) {
            std::cout << "  [OK] Списано " << qty << " шт.\n";
            perishable.increaseQuantity(-qty);
            
            std::cout << "  Обновить срок и пополнить? (1-да, 0-нет): ";
            std::cin >> ans;
            if (ans == 1) {
                std::cout << "  Срок годности новой партии (дней): ";
                int days; std::cin >> days;
                perishable.setExpiryDate(simulatedTime_ + days * 86400);
                
                std::cout << "  Количество: ";
                int addQty; std::cin >> addQty;
                if (addQty > 0) {
                    perishable.increaseQuantity(addQty);
                }
                std::cout << "  [OK] Обновлено. Срок: " << days << " дн., на складе: " << perishable.getQuantity() << " шт.\n";
            }
        }
        return;
    }
    
    // Случай 2: просрочен, но склад пуст
    if (isExpired && qty == 0) {
        std::cout << "  Статус: ПРОСРОЧЕН, склад пуст.\n";
        std::cout << "  Обновить срок и пополнить? (1-да, 0-нет): ";
        int ans; std::cin >> ans;
        if (ans == 1) {
            std::cout << "  Срок годности новой партии (дней): ";
            int days; std::cin >> days;
            perishable.setExpiryDate(simulatedTime_ + days * 86400);
            
            std::cout << "  Количество: ";
            int addQty; std::cin >> addQty;
            if (addQty > 0) {
                perishable.increaseQuantity(addQty);
            }
            std::cout << "  [OK] Обновлено. Срок: " << days << " дн., на складе: " << perishable.getQuantity() << " шт.\n";
        }
        return;
    }
    
    // Случай 3: свежий, есть на складе
    if (!isExpired && qty > 0) {
        std::cout << "  Статус: СВЕЖИЙ (годен ещё " << daysLeft << " дн.), на складе " << qty << " шт.\n";
        
        if (daysLeft <= 1) {
            std::cout << "  [!] Срок годности истекает. Обновить срок? (1-да, 0-нет): ";
            int ans; std::cin >> ans;
            if (ans == 1) {
                std::cout << "  [!] Старая партия (" << qty << " шт.) будет списана.\n";
                std::cout << "  Продолжить? (1-да, 0-нет): ";
                std::cin >> ans;
                if (ans == 1) {
                    perishable.increaseQuantity(-qty);
                    std::cout << "  Новый срок годности (дней): ";
                    int days; std::cin >> days;
                    perishable.setExpiryDate(simulatedTime_ + days * 86400);
                    
                    std::cout << "  Количество новой партии: ";
                    int addQty; std::cin >> addQty;
                    if (addQty > 0) {
                        perishable.increaseQuantity(addQty);
                    }
                    std::cout << "  [OK] Обновлено. Срок: " << days << " дн., на складе: " << perishable.getQuantity() << " шт.\n";
                }
            } else {
                std::cout << "  Сколько добавить к текущей партии? ";
                int addQty; std::cin >> addQty;
                if (addQty > 0) {
                    perishable.increaseQuantity(addQty);
                    std::cout << "  [OK] Добавлено " << addQty << " шт. Теперь " << perishable.getQuantity() << " шт.\n";
                }
            }
        } else {
            std::cout << "  Сколько добавить? ";
            int addQty; std::cin >> addQty;
            if (addQty > 0) {
                perishable.increaseQuantity(addQty);
                std::cout << "  [OK] Добавлено " << addQty << " шт. Теперь " << perishable.getQuantity() << " шт.\n";
            }
        }
        return;
    }
    
    // Случай 4: свежий, склад пуст
    if (!isExpired && qty == 0) {
        std::cout << "  Статус: СВЕЖИЙ (годен ещё " << daysLeft << " дн.), склад пуст.\n";
        std::cout << "  Сколько добавить? ";
        int addQty; std::cin >> addQty;
        if (addQty > 0) {
            perishable.increaseQuantity(addQty);
            std::cout << "  [OK] Добавлено " << addQty << " шт. Теперь " << perishable.getQuantity() << " шт.\n";
        }
    }
}

void VendingMachine::refillAll() {
    std::cout << "\n=== ПОПОЛНЕНИЕ ТОВАРОВ ===\n";
    
    for (auto& p : products_) {
        std::cout << "\n\"" << p->getName() << "\" (" << p->getType() << "): сейчас " << p->getQuantity() << " шт.";
        
        if (p->getType() == "Скоропортящийся") {
            Perishable* perishable = dynamic_cast<Perishable*>(p.get());
            refillPerishable(*perishable);
        } else {
            std::cout << "\n  Сколько добавить? ";
            int qty; std::cin >> qty;
            if (qty > 0) {
                p->increaseQuantity(qty);
                std::cout << "  [OK] Добавлено " << qty << " шт. Теперь " << p->getQuantity() << " шт.\n";
            }
        }
    }
    std::cout << "\n[OK] Пополнение завершено.\n";
}

void VendingMachine::addProduct() {
    std::cout << "\nДобавление товара:\n";
    std::cout << "Тип (1-Холодный, 2-Горячий, 3-Снек, 4-Скоропорт): ";
    int type; std::cin >> type;
    std::cout << "Название: ";
    std::string name; std::getline(std::cin >> std::ws, name);
    std::cout << "Цена: "; double price; std::cin >> price;
    std::cout << "Количество: "; int qty; std::cin >> qty;
    std::cout << "Срок годности (дней): "; int days; std::cin >> days;
    
    std::unique_ptr<Product> product;
    switch (type) {
        case 1: product = std::make_unique<ColdDrink>(name, price, qty, days); break;
        case 2: product = std::make_unique<HotDrink>(name, price, qty, days); break;
        case 3: product = std::make_unique<Snack>(name, price, qty, days); break;
        case 4: product = std::make_unique<Perishable>(name, price, qty, days); break;
        default: std::cout << "Неверный тип.\n"; return;
    }
    product->setExpiryDate(simulatedTime_ + days * 86400);
    products_.push_back(std::move(product));
    std::cout << "[OK] Товар \"" << name << "\" добавлен.\n";
}

void VendingMachine::removeProduct() {
    showProducts();
    std::cout << "Номер для удаления (0 - отмена): ";
    int idx; std::cin >> idx;
    if (idx == 0) return;
    if (idx < 1 || idx > static_cast<int>(products_.size())) {
        std::cout << "Неверный номер.\n"; return;
    }
    std::cout << "[OK] \"" << products_[static_cast<size_t>(idx - 1)]->getName() << "\" удален.\n";
    products_.erase(products_.begin() + idx - 1);
}

void VendingMachine::maintenanceMenu() {
    MaintenanceMode mm;
    bool maint = true;
    while (maint) {
        std::cout << "\n--- Техобслуживание ---\n";
        std::cout << "1. Пополнить товары (индивидуально)\n";
        std::cout << "2. Добавить новый товар\n";
        std::cout << "3. Удалить товар\n";
        std::cout << "4. Инвентаризация\n";
        std::cout << "5. Проверить свежесть\n";
        std::cout << "6. Проверить температуру\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";
        int c; std::cin >> c;
        switch (c) {
            case 1: refillAll(); break;
            case 2: addProduct(); break;
            case 3: removeProduct(); break;
            case 4: showInventory(); break;
            case 5: checkFreshness(); break;
            case 6: checkTemperature(); break;
            case 0: maint = false; break;
            default: std::cout << "Неверный выбор.\n";
        }
    }
}

void VendingMachine::run() {
    std::cout << "\n=== ТОРГОВЫЙ АВТОМАТ ===\n";
    std::cout << "Добро пожаловать!\n";
    std::cout << "[СИМУЛЯЦИЯ] Каждые 2 действия = +12 часов.\n";
    std::cout << "[СИМУЛЯЦИЯ] Напитки остывают, скоропорт портится.\n";
    
    bool running = true;
    while (running) {
        std::cout << "\n1. Купить товар\n";
        std::cout << "2. Статистика продаж\n";
        std::cout << "3. Проверить свежесть\n";
        std::cout << "4. Проверить температуру\n";
        std::cout << "5. Инвентаризация\n";
        std::cout << "6. Техобслуживание\n";
        std::cout << "7. Статус симуляции\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice != 0 && choice != 7) {
            advanceSimulation();
        }
        
        try {
            switch (choice) {
                case 1: selectProduct(); break;
                case 2: showStats(); break;
                case 3: checkFreshness(); break;
                case 4: checkTemperature(); break;
                case 5: showInventory(); break;
                case 6: maintenanceMenu(); break;
                case 7: showSimulationStatus(); break;
                case 0: running = false; break;
                default: std::cout << "Неверный выбор.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
            balance_ = 0;
        }
    }
}

void VendingMachineTask() {
    VendingMachine vm;
    vm.run();
}