#include "Task5_Airport.h"
#include <iostream>
#include <limits>

// ==================== Passengers ====================
EconomyPassenger::EconomyPassenger(const std::string& name) { name_ = name; }
std::string EconomyPassenger::getType() const { return "Эконом"; }
int EconomyPassenger::getPriority() const { return 3; }
double EconomyPassenger::getBaggageAllowance() const { return 23.0; }

BusinessPassenger::BusinessPassenger(const std::string& name) { name_ = name; }
std::string BusinessPassenger::getType() const { return "Бизнес"; }
int BusinessPassenger::getPriority() const { return 2; }
double BusinessPassenger::getBaggageAllowance() const { return 32.0; }

VipPassenger::VipPassenger(const std::string& name) { name_ = name; }
std::string VipPassenger::getType() const { return "VIP"; }
int VipPassenger::getPriority() const { return 1; }
double VipPassenger::getBaggageAllowance() const { return 50.0; }

// ==================== Ticket ====================
Ticket::Ticket(std::shared_ptr<Passenger> passenger, const std::string& flightNumber, double baggageWeight)
    : passenger_(passenger), flightNumber_(flightNumber), baggageWeight_(baggageWeight), 
      baggageFee_(0), onlineCheckIn_(false) {
    
    double allowance = passenger->getBaggageAllowance();
    if (baggageWeight > allowance) {
        baggageFee_ = (baggageWeight - allowance) * 500;
    }
}

// ==================== Flight ====================
Flight::Flight(const std::string& number, const std::string& destination, int capacity, time_t departureTime)
    : number_(number), destination_(destination), capacity_(capacity), 
      departureTime_(departureTime), delayed_(false) {}

bool Flight::addTicket(std::unique_ptr<Ticket> ticket) {
    if (isFull()) {
        return false;
    }
    tickets_.push_back(std::move(ticket));
    return true;
}

void Flight::board() {
    std::sort(tickets_.begin(), tickets_.end(),
        [](const auto& a, const auto& b) {
            return a->getPassenger()->getPriority() < b->getPassenger()->getPriority();
        });
    
    std::cout << "\n=== Посадка на рейс " << number_ << " (" << destination_ << ") ===\n";
    
    if (delayed_) {
        std::cout << "[!] Рейс задержан: " << delayReason_ << "\n";
    }
    
    std::cout << "Пассажиры в порядке приоритета:\n";
    for (size_t i = 0; i < tickets_.size(); ++i) {
        const auto& t = tickets_[i];
        std::cout << "  " << (i + 1) << ". " << t->getPassenger()->getName()
                  << " (" << t->getPassenger()->getType() << ")";
        
        if (t->isOnlineCheckIn()) {
            std::cout << " [онлайн-регистрация]";
        }
        
        if (t->getBaggageFee() > 0) {
            std::cout << " | Доплата за багаж: " << t->getBaggageFee() << " руб.";
        }
        
        std::cout << "\n";
    }
}

void Flight::delay(const std::string& reason) {
    delayed_ = true;
    delayReason_ = reason;
    departureTime_ += 3600;
    std::cout << "[!] Рейс " << number_ << " задержан: " << delayReason_ << "\n";
    time_t dep = departureTime_;
    std::cout << "[!] Новое время вылета: " << ctime(&dep);
}

void Flight::showLoad() const {
    double pct = getLoadPercentage();
    std::cout << "  " << number_ << " (" << destination_ << "): " 
              << tickets_.size() << "/" << capacity_ << " (" << pct << "%)";
    if (delayed_) std::cout << " [ЗАДЕРЖАН]";
    std::cout << "\n";
}

double Flight::getLoadPercentage() const {
    if (capacity_ == 0) return 0;
    return static_cast<double>(tickets_.size()) / capacity_ * 100;
}

// ==================== Transfer ====================
Transfer::Transfer(const std::string& fromFlight, const std::string& toFlight, int minTransferTime)
    : fromFlight_(fromFlight), toFlight_(toFlight), minTransferTime_(minTransferTime) {}

bool Transfer::canMakeTransfer(time_t arrivalTime, time_t departureTime) const {
    double diff = difftime(departureTime, arrivalTime);
    return diff >= minTransferTime_ * 60;
}

// ==================== Airport ====================
Airport::Airport() {
    setupDemo();
}

void Airport::setupDemo() {
    time_t now = time(nullptr);
    
    flights_.push_back(std::make_unique<Flight>("SU100", "Москва", 3, now + 7200));
    flights_.push_back(std::make_unique<Flight>("SU200", "Сочи", 3, now + 10800));
    flights_.push_back(std::make_unique<Flight>("SU300", "Казань", 2, now + 18000));
    
    transfers_.push_back(Transfer("SU100", "SU200", 60));
    transfers_.push_back(Transfer("SU100", "SU300", 90));
    transfers_.push_back(Transfer("SU200", "SU300", 45));
    
    auto p1 = std::make_shared<VipPassenger>("Анна Петрова");
    auto t1 = std::make_unique<Ticket>(p1, "SU100", 15);
    flights_[0]->addTicket(std::move(t1));
    
    auto p2 = std::make_shared<BusinessPassenger>("Сергей Иванов");
    auto t2 = std::make_unique<Ticket>(p2, "SU100", 35);
    flights_[0]->addTicket(std::move(t2));
}

Flight* Airport::findFlight(const std::string& number) {
    auto it = std::find_if(flights_.begin(), flights_.end(),
        [&number](const auto& f) { return f->getNumber() == number; });
    return (it != flights_.end()) ? it->get() : nullptr;
}

Flight* Airport::findFlight(const std::string& number) const {
    auto it = std::find_if(flights_.begin(), flights_.end(),
        [&number](const auto& f) { return f->getNumber() == number; });
    return (it != flights_.end()) ? it->get() : nullptr;
}

void Airport::showFlights() const {
    std::cout << "\n=== РАСПИСАНИЕ РЕЙСОВ ===\n";
    for (const auto& f : flights_) {
        f->showLoad();
    }
}

void Airport::showFlightDetails() const {
    showFlights();
    std::cout << "Номер рейса: ";
    std::string num;
    std::getline(std::cin >> std::ws, num);
    
    Flight* f = findFlight(num);
    if (!f) {
        std::cout << "Рейс не найден.\n";
        return;
    }
    
    std::cout << "\nРейс: " << f->getNumber() << "\n";
    std::cout << "Направление: " << f->getDestination() << "\n";
    std::cout << "Вместимость: " << f->getCapacity() << "\n";
    std::cout << "Загруженность: " << f->getLoadPercentage() << "%\n";
    time_t dep = f->getDepartureTime();
    std::cout << "Вылет: " << ctime(&dep);
    if (f->isDelayed()) {
        std::cout << "Статус: ЗАДЕРЖАН (" << f->getDelayReason() << ")\n";
    } else {
        std::cout << "Статус: По расписанию\n";
    }
}

void Airport::registerPassenger() {
    std::cout << "Имя пассажира: ";
    std::string name;
    std::getline(std::cin >> std::ws, name);
    
    std::cout << "Тип пассажира:\n  1 - Эконом (багаж 23 кг)\n  2 - Бизнес (багаж 32 кг)\n  3 - VIP (багаж 50 кг)\nВыбор: ";
    int type;
    std::cin >> type;
    
    std::shared_ptr<Passenger> passenger;
    switch (type) {
        case 1: passenger = std::make_shared<EconomyPassenger>(name); break;
        case 2: passenger = std::make_shared<BusinessPassenger>(name); break;
        case 3: passenger = std::make_shared<VipPassenger>(name); break;
        default: std::cout << "Неверный тип.\n"; return;
    }
    
    showFlights();
    std::cout << "Номер рейса: ";
    std::string flightNum;
    std::getline(std::cin >> std::ws, flightNum);
    
    Flight* flight = findFlight(flightNum);
    if (!flight) {
        std::cout << "Рейс не найден.\n";
        return;
    }
    
    std::cout << "Вес багажа (кг): ";
    double weight;
    std::cin >> weight;
    
    auto ticket = std::make_unique<Ticket>(passenger, flightNum, weight);
    
    if (ticket->getBaggageFee() > 0) {
        std::cout << "Перевес! Доплата: " << ticket->getBaggageFee() << " руб.\n";
        std::cout << "Продолжить регистрацию? (1-да, 0-нет): ";
        int ans; std::cin >> ans;
        if (ans != 1) return;
    }
    
    std::cout << "Тип регистрации:\n  1 - В аэропорту\n  2 - Онлайн\nВыбор: ";
    int regType; std::cin >> regType;
    if (regType == 2) {
        ticket->setOnlineCheckIn(true);
    }
    
    if (flight->addTicket(std::move(ticket))) {
        std::cout << "[OK] " << name << " зарегистрирован на рейс " << flightNum;
        if (regType == 2) std::cout << " (онлайн)";
        std::cout << ".\n";
    } else {
        std::cout << "[!] Рейс полный. Регистрация невозможна.\n";
    }
}

void Airport::registerOnline() {
    std::cout << "=== Онлайн-регистрация ===\n";
    std::cout << "Имя пассажира: ";
    std::string name;
    std::getline(std::cin >> std::ws, name);
    
    std::cout << "Номер рейса: ";
    std::string num;
    std::getline(std::cin, num);
    
    Flight* f = findFlight(num);
    if (!f) {
        std::cout << "Рейс не найден.\n";
        return;
    }
    
    bool found = false;
    for (auto& t : f->getTickets()) {
        if (t->getPassenger()->getName() == name && !t->isOnlineCheckIn()) {
            t->setOnlineCheckIn(true);
            std::cout << "[OK] Онлайн-регистрация для " << name << " на рейс " << num << " выполнена.\n";
            found = true;
            break;
        }
    }
    
    if (!found) {
        std::cout << "[!] Пассажир не найден или уже зарегистрирован онлайн.\n";
    }
}

void Airport::boardFlight() {
    showFlights();
    std::cout << "Номер рейса для посадки: ";
    std::string num;
    std::getline(std::cin >> std::ws, num);
    
    Flight* f = findFlight(num);
    if (!f) {
        std::cout << "Рейс не найден.\n";
        return;
    }
    
    f->board();
}

void Airport::delayFlight() {
    showFlights();
    std::cout << "Номер рейса для задержки: ";
    std::string num;
    std::getline(std::cin >> std::ws, num);
    
    Flight* f = findFlight(num);
    if (!f) {
        std::cout << "Рейс не найден.\n";
        return;
    }
    
    std::cout << "Причина задержки: ";
    std::string reason;
    std::getline(std::cin, reason);
    
    f->delay(reason);
}

void Airport::checkTransfer() {
    std::cout << "\n=== ПРОВЕРКА ТРАНСФЕРОВ ===\n";
    
    if (transfers_.empty()) {
        std::cout << "Трансферных маршрутов нет.\n";
        return;
    }
    
    for (const auto& tr : transfers_) {
        Flight* from = findFlight(tr.getFromFlight());
        Flight* to = findFlight(tr.getToFlight());
        
        if (from && to) {
            bool canMake = tr.canMakeTransfer(from->getDepartureTime(), to->getDepartureTime());
            std::cout << tr.getFromFlight() << " -> " << tr.getToFlight() 
                      << " (мин. стыковка: " << tr.getMinTransferTime() << " мин): ";
            if (canMake) {
                std::cout << "УСПЕВАЕТ\n";
            } else {
                std::cout << "НЕ УСПЕВАЕТ\n";
            }
        }
    }
}

void Airport::showStats() const {
    std::cout << "\n=== СТАТИСТИКА АЭРОПОРТА ===\n";
    
    int totalBooked = 0;
    int totalCapacity = 0;
    int delayedCount = 0;
    
    for (const auto& f : flights_) {
        f->showLoad();
        totalBooked += f->getBooked();
        totalCapacity += f->getCapacity();
        if (f->isDelayed()) delayedCount++;
    }
    
    double overallLoad = totalCapacity > 0 ? static_cast<double>(totalBooked) / totalCapacity * 100 : 0;
    
    std::cout << "\nВсего рейсов: " << flights_.size() << "\n";
    std::cout << "Задержано рейсов: " << delayedCount << "\n";
    std::cout << "Общая загруженность: " << totalBooked << "/" << totalCapacity 
              << " (" << overallLoad << "%)\n";
    
    int vipCount = 0, bizCount = 0, ecoCount = 0;
    double totalBaggageFees = 0;
    
    for (const auto& f : flights_) {
        for (const auto& t : f->getTickets()) {
            std::string type = t->getPassenger()->getType();
            if (type == "VIP") vipCount++;
            else if (type == "Бизнес") bizCount++;
            else ecoCount++;
            
            totalBaggageFees += t->getBaggageFee();
        }
    }
    
    std::cout << "\nПассажиры: VIP=" << vipCount << ", Бизнес=" << bizCount 
              << ", Эконом=" << ecoCount << ", Всего=" << (vipCount + bizCount + ecoCount) << "\n";
    std::cout << "Доплаты за багаж: " << totalBaggageFees << " руб.\n";
}

void Airport::run() {
    std::cout << "\n=== АЭРОПОРТ ===\n";
    
    bool running = true;
    while (running) {
        std::cout << "\n1. Зарегистрировать пассажира\n";
        std::cout << "2. Онлайн-регистрация\n";
        std::cout << "3. Посадка на рейс\n";
        std::cout << "4. Задержать рейс\n";
        std::cout << "5. Информация о рейсе\n";
        std::cout << "6. Проверить трансферы\n";
        std::cout << "7. Статистика аэропорта\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";
        
        int choice;
        std::cin >> choice;
        
        try {
            switch (choice) {
                case 1: registerPassenger(); break;
                case 2: registerOnline(); break;
                case 3: boardFlight(); break;
                case 4: delayFlight(); break;
                case 5: showFlightDetails(); break;
                case 6: checkTransfer(); break;
                case 7: showStats(); break;
                case 0: running = false; break;
                default: std::cout << "Неверный выбор.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
    }
}

void AirportTask() {
    Airport airport;
    airport.run();
}