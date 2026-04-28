#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <memory>
#include <ctime>
#include <algorithm>
#include <map>
#include <cmath>
#include <cstdlib>
#include <clocale>
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::move;
using std::ostream;
using std::istream;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::ostringstream;
using std::fixed;
using std::setprecision;
using std::to_string;
using std::numeric_limits;
using std::streamsize;
using std::map;
using std::abs;
using std::floor;


// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidatedInt(const string& prompt, int min, int max) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            cout << "Ошибка! Введите число от " << min << " до " << max << ": ";
            continue;
        }
        istringstream ss(input);
        double temp;
        ss >> temp;
        if (ss.eof() && std::abs(temp - std::floor(temp)) < 1e-9 && temp >= min && temp <= max) {
            return static_cast<int>(temp);
        }
        cout << "Ошибка! Введите целое число от " << min << " до " << max << ": ";
    }
}

double getValidatedDouble(const string& prompt, double min, double max) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            cout << "Ошибка! Введите число от " << min << " до " << max << ": ";
            continue;
        }
        istringstream ss(input);
        double value;
        ss >> value;
        if (ss.eof() && value >= min && value <= max) {
            return value;
        }
        cout << "Ошибка! Введите число от " << min << " до " << max << ": ";
    }
}

string getValidatedString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) return input;
        cout << "Ошибка! Строка не может быть пустой.\n";
    }
}

void executeTask(void (*taskFunc)(), const string& taskName) {
    int choice;
    do {
        cout << "\n=== " << taskName << " ===\n";
        taskFunc();
        cout << "\nЗадача завершена.\n";
        cout << "1. Выполнить задачу еще раз\n";
        cout << "2. Вернуться в главное меню\n";
        choice = getValidatedInt("Выберите действие: ", 1, 2);
    } while (choice == 1);
}

// ============================================
// ЗАДАЧА 1: СИСТЕМА БРОНИРОВАНИЯ БИЛЕТОВ
// ============================================

// Вспомогательные функции валидации для задачи 1

// Проверка формата даты ДД.ММ.ГГГГ
bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '.' || date[5] != '.') return false;
    
    for (string::size_type i = 0; i < date.length(); i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(static_cast<unsigned char>(date[i]))) return false;
    }
    
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    
    if (year < 2024 || year > 2030) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (leap && day > 29) return false;
        if (!leap && day > 28) return false;
    }
    return true;
}

string getValidatedDate(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (isValidDate(input)) return input;
        cout << "Ошибка! Введите дату в формате ДД.ММ.ГГГГ (например, 25.12.2024).\n";
    }
}

// Проверка класса обслуживания для авиабилетов
string getValidatedFlightClass(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input == "Эконом" || input == "эконом" || 
            input == "Бизнес" || input == "бизнес" || 
            input == "Первый" || input == "первый") {
            return input;
        }
        cout << "Ошибка! Введите один из классов: Эконом, Бизнес, Первый.\n";
    }
}

// Проверка типа места для ЖД билетов
string getValidatedSeatType(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input == "Плацкарт" || input == "плацкарт" ||
            input == "Купе" || input == "купе" ||
            input == "СВ" || input == "св" ||
            input == "Сидячий" || input == "сидячий") {
            return input;
        }
        cout << "Ошибка! Введите один из типов: Плацкарт, Купе, СВ, Сидячий.\n";
    }
}

// Проверка ввода да/нет для багажа
bool getValidatedBool(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input == "да" || input == "Да" || input == "ДА" || 
            input == "yes" || input == "Yes" || input == "YES" ||
            input == "y" || input == "Y" || input == "1") return true;
        if (input == "нет" || input == "Нет" || input == "НЕТ" || 
            input == "no" || input == "No" || input == "NO" ||
            input == "n" || input == "N" || input == "0") return false;
        cout << "Ошибка! Введите да/нет (y/n).\n";
    }
}

// Проверка названия фильма (только буквы, цифры, пробелы, дефисы, двоеточия)
string getValidatedMovieName(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            cout << "Ошибка! Название фильма не может быть пустым.\n";
            continue;
        }
        bool valid = true;
        for (char c : input) {
            if (!isalnum(static_cast<unsigned char>(c)) && c != ' ' && c != '-' && c != ':' && c != '.' && 
                c != '!' && c != '?' && c != '\'' && c != '"' && c != '&') {
                valid = false;
                break;
            }
        }
        if (valid) return input;
        cout << "Ошибка! Название содержит недопустимые символы.\n";
    }
}

// Проверка номера рейса (буквы + цифры)
string getValidatedFlightNumber(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            cout << "Ошибка! Номер рейса не может быть пустым.\n";
            continue;
        }
        bool valid = true;
        for (char c : input) {
            if (!isalnum(static_cast<unsigned char>(c)) && c != '-') {
                valid = false;
                break;
            }
        }
        if (valid) return input;
        cout << "Ошибка! Номер рейса должен содержать только буквы и цифры.\n";
    }
}

// Проверка номера поезда (цифры + буквы)
string getValidatedTrainNumber(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            cout << "Ошибка! Номер поезда не может быть пустым.\n";
            continue;
        }
        bool valid = true;
        for (char c : input) {
            if (!isalnum(static_cast<unsigned char>(c)) && c != '-') {
                valid = false;
                break;
            }
        }
        if (valid) return input;
        cout << "Ошибка! Номер поезда должен содержать только цифры и буквы.\n";
    }
}

// Перечисление статусов билета
enum class TicketStatus {
    BOOKED,    // забронирован
    PAID,      // оплачен
    REFUNDED   // возвращен
};

// Вспомогательная функция для вывода статуса
string statusToString(TicketStatus status) {
    switch (status) {
        case TicketStatus::BOOKED:   return "Забронирован";
        case TicketStatus::PAID:     return "Оплачен";
        case TicketStatus::REFUNDED: return "Возвращен";
        default: return "Неизвестно";
    }
}

// Базовый абстрактный класс Ticket
class Ticket {
protected:
    static int totalTicketsSold;  // статический счетчик
    static int nextTicketId;      // автоинкремент ID
    
    int ticketId;
    string eventDate;
    double basePrice;
    TicketStatus status;
    
public:
    // Конструктор по умолчанию
    Ticket() : ticketId(++nextTicketId), eventDate("01.01.2025"), basePrice(0.0), status(TicketStatus::BOOKED) {
        totalTicketsSold++;
        cout << "[Ticket] Создан билет #" << ticketId << " (базовый конструктор)\n";
    }
    
    // Параметризованный конструктор
    Ticket(const string& date, double price)
        : ticketId(++nextTicketId), eventDate(date), basePrice(price), status(TicketStatus::BOOKED) {
        totalTicketsSold++;
        cout << "[Ticket] Создан билет #" << ticketId << " (параметризованный конструктор)\n";
    }
    
    // Конструктор копирования
    Ticket(const Ticket& other)
        : ticketId(++nextTicketId), eventDate(other.eventDate), basePrice(other.basePrice), status(other.status) {
        totalTicketsSold++;
        cout << "[Ticket] Скопирован билет (новый ID #" << ticketId << ")\n";
    }
    
    // Виртуальный деструктор
    virtual ~Ticket() {
        totalTicketsSold--;
        cout << "[Ticket] Удален билет #" << ticketId << " | Всего осталось: " << totalTicketsSold << "\n";
    }
    
    // Чисто виртуальные методы
    virtual double calculateFinalPrice() const = 0;
    virtual bool canRefund() const = 0;
    virtual string getDescription() const = 0;
    
    // Общие методы
    int getId() const { return ticketId; }
    double getBasePrice() const { return basePrice; }
    TicketStatus getStatus() const { return status; }
    
    void setStatus(TicketStatus newStatus) {
        status = newStatus;
    }
    
    // Оплата билета
    bool pay() {
        if (status == TicketStatus::BOOKED) {
            status = TicketStatus::PAID;
            cout << "[Ticket] Билет #" << ticketId << " оплачен.\n";
            return true;
        }
        cout << "[Ticket] Нельзя оплатить билет #" << ticketId << " (статус: " << statusToString(status) << ").\n";
        return false;
    }
    
    // Возврат билета
    bool refund() {
        if (!canRefund()) {
            cout << "[Ticket] Билет #" << ticketId << " нельзя вернуть (статус: " << statusToString(status) << ").\n";
            return false;
        }
        status = TicketStatus::REFUNDED;
        cout << "[Ticket] Билет #" << ticketId << " возвращен. Деньги будут возвращены.\n";
        return true;
    }
    
    static int getTotalTicketsSold() { return totalTicketsSold; }
};

// Инициализация статических полей
int Ticket::totalTicketsSold = 0;
int Ticket::nextTicketId = 0;

// Класс MovieTicket (кино)
class MovieTicket : public Ticket {
private:
    string movieName;
    int hall;
    int row;
    int seat;
    
public:
    MovieTicket(const string& date, double price, const string& movie, int h, int r, int s)
        : Ticket(date, price), movieName(movie), hall(h), row(r), seat(s) {
        cout << "  [MovieTicket] Фильм \"" << movieName << "\", зал " << hall << ", ряд " << row << ", место " << seat << "\n";
    }
    
    ~MovieTicket() override {
        cout << "  [MovieTicket] Удален билет на фильм \"" << movieName << "\"\n";
    }
    
    double calculateFinalPrice() const override {
        // Сбор за бронирование 10%
        return basePrice * 1.10;
    }
    
    bool canRefund() const override {
        // Можно вернуть, если не оплачен
        return status != TicketStatus::PAID;
    }
    
    string getDescription() const override {
        ostringstream oss;
        oss << "КИНО | " << movieName << " | " << eventDate
            << " | Зал " << hall << " Ряд " << row << " Место " << seat
            << " | Базовая: " << fixed << setprecision(2) << basePrice
            << " | Итоговая: " << calculateFinalPrice()
            << " | Статус: " << statusToString(status);
        return oss.str();
    }
};

// Класс FlightTicket (авиа)
class FlightTicket : public Ticket {
private:
    string flightNumber;
    string seatClass;    // "Эконом", "Бизнес", "Первый"
    bool hasBaggage;
    
public:
    FlightTicket(const string& date, double price, const string& flight, const string& sClass, bool baggage)
        : Ticket(date, price), flightNumber(flight), seatClass(sClass), hasBaggage(baggage) {
        cout << "  [FlightTicket] Рейс " << flightNumber << ", класс \"" << seatClass << "\", багаж: " << (baggage ? "да" : "нет") << "\n";
    }
    
    ~FlightTicket() override {
        cout << "  [FlightTicket] Удален билет на рейс " << flightNumber << "\n";
    }
    
    double calculateFinalPrice() const override {
        double final = basePrice;
        // Наценка за класс
        if (seatClass == "Бизнес" || seatClass == "бизнес") final *= 2.0;
        else if (seatClass == "Первый" || seatClass == "первый") final *= 4.0;
        // Сбор за багаж
        if (hasBaggage) final += 1500.0;
        // Топливный сбор 5%
        final *= 1.05;
        return final;
    }
    
    bool canRefund() const override {
        // Авиабилеты можно вернуть только если они не оплачены
        return status == TicketStatus::BOOKED;
    }
    
    string getDescription() const override {
        ostringstream oss;
        oss << "АВИА | Рейс " << flightNumber << " | " << eventDate
            << " | Класс: " << seatClass
            << " | Багаж: " << (hasBaggage ? "да" : "нет")
            << " | Базовая: " << fixed << setprecision(2) << basePrice
            << " | Итоговая: " << calculateFinalPrice()
            << " | Статус: " << statusToString(status);
        return oss.str();
    }
};

// Класс TrainTicket (ЖД)
class TrainTicket : public Ticket {
private:
    string trainNumber;
    int carriage;
    string seatType;  // "Плацкарт", "Купе", "СВ", "Сидячий"
    
public:
    TrainTicket(const string& date, double price, const string& train, int car, const string& type)
        : Ticket(date, price), trainNumber(train), carriage(car), seatType(type) {
        cout << "  [TrainTicket] Поезд " << trainNumber << ", вагон " << carriage << ", тип \"" << seatType << "\"\n";
    }
    
    ~TrainTicket() override {
        cout << "  [TrainTicket] Удален билет на поезд " << trainNumber << "\n";
    }
    
    double calculateFinalPrice() const override {
        double final = basePrice;
        // Наценка за тип места
        if (seatType == "Купе" || seatType == "купе") final *= 1.5;
        else if (seatType == "СВ" || seatType == "св") final *= 3.0;
        else if (seatType == "Сидячий" || seatType == "сидячий") final *= 0.8;
        // Сервисный сбор 7%
        final *= 1.07;
        return final;
    }
    
    bool canRefund() const override {
        // ЖД билеты всегда можно вернуть (по закону)
        return true;
    }
    
    string getDescription() const override {
        ostringstream oss;
        oss << "ЖД | Поезд " << trainNumber << " | " << eventDate
            << " | Вагон " << carriage << " | Тип: " << seatType
            << " | Базовая: " << fixed << setprecision(2) << basePrice
            << " | Итоговая: " << calculateFinalPrice()
            << " | Статус: " << statusToString(status);
        return oss.str();
    }
};

// Система бронирования
class BookingSystem {
private:
    vector<unique_ptr<Ticket>> tickets;
    
public:
    void addMovieTicket() {
        string date = getValidatedDate("Введите дату (ДД.ММ.ГГГГ): ");
        double price = getValidatedDouble("Базовая цена (1-100000): ", 1.0, 100000.0);
        string movie = getValidatedMovieName("Название фильма: ");
        int hall = getValidatedInt("Зал (1-20): ", 1, 20);
        int row = getValidatedInt("Ряд (1-30): ", 1, 30);
        int seat = getValidatedInt("Место (1-50): ", 1, 50);
        
        auto ticket = make_unique<MovieTicket>(date, price, movie, hall, row, seat);
        cout << "\nБилет добавлен! Итоговая цена со сборами: " 
             << fixed << setprecision(2) << ticket->calculateFinalPrice() << " руб.\n";
        cout << "Всего билетов в системе: " << Ticket::getTotalTicketsSold() << "\n";
        tickets.push_back(move(ticket));
        displayStatusDistribution();
    }
    
    void addFlightTicket() {
        string date = getValidatedDate("Введите дату (ДД.ММ.ГГГГ): ");
        double price = getValidatedDouble("Базовая цена (1-1000000): ", 1.0, 1000000.0);
        string flight = getValidatedFlightNumber("Номер рейса (буквы и цифры): ");
        string seatClass = getValidatedFlightClass("Класс обслуживания (Эконом/Бизнес/Первый): ");
        bool baggage = getValidatedBool("Багаж (да/нет): ");
        
        auto ticket = make_unique<FlightTicket>(date, price, flight, seatClass, baggage);
        cout << "\nБилет добавлен! Итоговая цена: " 
             << fixed << setprecision(2) << ticket->calculateFinalPrice() << " руб.\n";
        cout << "Всего билетов в системе: " << Ticket::getTotalTicketsSold() << "\n";
        tickets.push_back(move(ticket));
        displayStatusDistribution();
    }
    
    void addTrainTicket() {
        string date = getValidatedDate("Введите дату (ДД.ММ.ГГГГ): ");
        double price = getValidatedDouble("Базовая цена (1-100000): ", 1.0, 100000.0);
        string train = getValidatedTrainNumber("Номер поезда (цифры и буквы): ");
        int carriage = getValidatedInt("Вагон (1-20): ", 1, 20);
        string seatType = getValidatedSeatType("Тип места (Плацкарт/Купе/СВ/Сидячий): ");
        
        auto ticket = make_unique<TrainTicket>(date, price, train, carriage, seatType);
        cout << "\nБилет добавлен! Итоговая цена: " 
             << fixed << setprecision(2) << ticket->calculateFinalPrice() << " руб.\n";
        cout << "Всего билетов в системе: " << Ticket::getTotalTicketsSold() << "\n";
        tickets.push_back(move(ticket));
        displayStatusDistribution();
    }
    
    void payForTicket() {
        if (tickets.empty()) {
            cout << "\nНет билетов для оплаты.\n";
            return;
        }
        
        // Показываем только билеты, которые можно оплатить
        cout << "\n=== БИЛЕТЫ, ДОСТУПНЫЕ ДЛЯ ОПЛАТЫ ===\n";
        cout << string(80, '-') << endl;
        bool found = false;
        for (vector<unique_ptr<Ticket>>::size_type i = 0; i < tickets.size(); ++i) {
            if (tickets[i]->getStatus() == TicketStatus::BOOKED) {
                cout << (i + 1) << ". " << tickets[i]->getDescription() << "\n";
                found = true;
            }
        }
        if (!found) {
            cout << "Нет неоплаченных билетов.\n";
            return;
        }
        cout << string(80, '-') << endl;
        
        int indexInt = getValidatedInt("Введите номер билета для оплаты (0-выход): ", 0, static_cast<int>(tickets.size())) - 1;
        if (indexInt < 0) return;
        
        vector<unique_ptr<Ticket>>::size_type index = static_cast<vector<unique_ptr<Ticket>>::size_type>(indexInt);
        
        if (tickets[index]->pay()) {
            displayStatusDistribution();
        }
    }
    
    void showAllTickets() const {
        if (tickets.empty()) {
            cout << "\nНет забронированных билетов.\n";
            return;
        }
        cout << "\n=== ВСЕ БИЛЕТЫ (" << tickets.size() << ") ===\n";
        cout << string(80, '-') << endl;
        for (vector<unique_ptr<Ticket>>::size_type i = 0; i < tickets.size(); ++i) {
            cout << (i + 1) << ". " << tickets[i]->getDescription() << "\n";
        }
        cout << string(80, '-') << endl;
        displayStatusDistribution();
    }
    
    void cancelBooking() {
        if (tickets.empty()) {
            cout << "\nНет билетов для отмены.\n";
            return;
        }
        showAllTickets();
        int indexInt = getValidatedInt("Введите номер билета для отмены (0-выход): ", 0, static_cast<int>(tickets.size())) - 1;
        if (indexInt < 0) return;
        
        vector<unique_ptr<Ticket>>::size_type index = static_cast<vector<unique_ptr<Ticket>>::size_type>(indexInt);
        
        if (tickets[index]->refund()) {
            displayStatusDistribution();
            // Удаляем возвращённый билет из системы
            tickets.erase(tickets.begin() + static_cast<long>(index));
            cout << "Билет удален из системы.\n";
        }
    }
    
    void showTotalRevenue() const {
        double total = 0.0;
        int paidCount = 0;
        for (const auto& ticket : tickets) {
            if (ticket->getStatus() == TicketStatus::PAID) {
                total += ticket->calculateFinalPrice();
                paidCount++;
            }
        }
        cout << "\n=== ФИНАНСОВАЯ СТАТИСТИКА ===\n";
        cout << "Оплаченных билетов: " << paidCount << "\n";
        cout << "Общая выручка: " << fixed << setprecision(2) << total << " руб.\n";
        cout << "Всего билетов в системе: " << Ticket::getTotalTicketsSold() << "\n";
    }
    
    void displayStatusDistribution() const {
        int booked = 0, paid = 0, refunded = 0;
        for (const auto& ticket : tickets) {
            switch (ticket->getStatus()) {
                case TicketStatus::BOOKED: booked++; break;
                case TicketStatus::PAID: paid++; break;
                case TicketStatus::REFUNDED: refunded++; break;
            }
        }
        cout << "По статусам: забронировано " << booked 
             << ", оплачено " << paid 
             << ", возвращено " << refunded << "\n";
    }
    
    void run() {
        int choice;
        do {
            cout << "\n=== СИСТЕМА БРОНИРОВАНИЯ ===\n";
            cout << "1. Добавить билет в кино\n";
            cout << "2. Добавить авиабилет\n";
            cout << "3. Добавить ЖД билет\n";
            cout << "4. Показать все билеты\n";
            cout << "5. Оплатить билет\n";
            cout << "6. Отменить бронирование (возврат)\n";
            cout << "7. Общая выручка\n";
            cout << "0. Вернуться в главное меню\n";
            choice = getValidatedInt("Выберите действие: ", 0, 7);
            
            switch (choice) {
                case 1: addMovieTicket(); break;
                case 2: addFlightTicket(); break;
                case 3: addTrainTicket(); break;
                case 4: showAllTickets(); break;
                case 5: payForTicket(); break;
                case 6: cancelBooking(); break;
                case 7: showTotalRevenue(); break;
            }
        } while (choice != 0);
    }
};

void task1_BookingSystem() {
    BookingSystem system;
    system.run();
}

// ============================================
// ЗАДАЧА 2: СИСТЕМА РАСЧЕТА ЗАРПЛАТЫ
// ============================================

int getValidatedPin(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (input.length() != 4) {
            cout << "Ошибка! PIN должен содержать ровно 4 цифры.\n";
            continue;
        }
        
        bool allDigits = true;
        for (char c : input) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                allDigits = false;
                break;
            }
        }
        
        if (!allDigits) {
            cout << "Ошибка! PIN должен содержать только цифры.\n";
            continue;
        }
        
        return stoi(input);
    }
}

// Базовый класс Employee
class Employee {
protected:
    string name;
    int id;
    static int totalEmployees;
    
    // Статический набор для отслеживания занятых ID
    static vector<int> usedIds;
    
public:
    Employee() : name("Неизвестный"), id(0) {
        totalEmployees++;
        cout << "[Employee] Создан сотрудник (конструктор по умолчанию)\n";
    }
    
    Employee(const string& n, int i) : name(n), id(i) {
        usedIds.push_back(id);
        totalEmployees++;
        cout << "[Employee] Создан сотрудник #" << id << ": " << name << "\n";
    }
    
    // Конструктор копирования (глубокое копирование строки, новый ID)
    Employee(const Employee& other) : name(other.name), id(other.id + 1000) {
        usedIds.push_back(id);
        totalEmployees++;
        cout << "[Employee] Скопирован сотрудник (новый ID #" << id << ")\n";
    }
    
    virtual ~Employee() {
        auto it = std::find(usedIds.begin(), usedIds.end(), id);
        if (it != usedIds.end()) usedIds.erase(it);
        totalEmployees--;
        cout << "[Employee] Удален сотрудник #" << id << ": " << name << "\n";
    }
    
    virtual double calculateSalary() const = 0;
    virtual string getType() const = 0;
    
    int getId() const { return id; }
    string getName() const { return name; }
    static int getTotalEmployees() { return totalEmployees; }
    
    static bool isIdUsed(int idToCheck) {
        return std::find(usedIds.begin(), usedIds.end(), idToCheck) != usedIds.end();
    }
    
    static int suggestNewId() {
        int newId = 1;
        while (isIdUsed(newId)) newId++;
        return newId;
    }
};

int Employee::totalEmployees = 0;
vector<int> Employee::usedIds;

// Штатный сотрудник (оклад + фиксированная премия)
class SalariedEmployee : public Employee {
private:
    double salary;
    double fixedBonus;
    
public:
    SalariedEmployee(const string& n, int i, double s, double bonus)
        : Employee(n, i), salary(s), fixedBonus(bonus) {
        cout << "  [Salaried] Оклад " << salary << " + премия " << fixedBonus << " руб.\n";
    }
    
    ~SalariedEmployee() override {
        cout << "  [Salaried] Удален штатный сотрудник\n";
    }
    
    double calculateSalary() const override {
        return salary + fixedBonus;
    }
    
    string getType() const override {
        return "Штатный сотрудник (оклад + премия)";
    }
};

// Почасовой сотрудник
class HourlyEmployee : public Employee {
private:
    double hourlyRate;
    int hoursWorked;
    
public:
    HourlyEmployee(const string& n, int i, double hRate, int hours)
        : Employee(n, i), hourlyRate(hRate), hoursWorked(hours) {
        cout << "  [Hourly] Ставка " << hourlyRate << " руб/час, отработано " << hoursWorked << " ч.\n";
    }
    
    ~HourlyEmployee() override {
        cout << "  [Hourly] Удален почасовой сотрудник\n";
    }
    
    double calculateSalary() const override {
        return hourlyRate * hoursWorked;
    }
    
    string getType() const override {
        return "Почасовой сотрудник";
    }
};

// Менеджер (оклад + процент от продаж)
class Manager : public Employee {
private:
    double baseSalary;
    double salesPercentage;
    double departmentSales;
    
public:
    Manager(const string& n, int i, double s, double percent, double sales)
        : Employee(n, i), baseSalary(s), salesPercentage(percent), departmentSales(sales) {
        cout << "  [Manager] Оклад " << baseSalary << " + " << salesPercentage << "% от продаж (" << departmentSales << " руб.)\n";
    }
    
    ~Manager() override {
        cout << "  [Manager] Удален менеджер\n";
    }
    
    double calculateSalary() const override {
        return baseSalary + (departmentSales * salesPercentage / 100.0);
    }
    
    string getType() const override {
        return "Менеджер (оклад + % от продаж)";
    }
};

// Фрилансер
class Freelancer : public Employee {
private:
    double projectPayment;
    
public:
    Freelancer(const string& n, int i, double payment)
        : Employee(n, i), projectPayment(payment) {
        cout << "  [Freelancer] Оплата за проект: " << projectPayment << " руб.\n";
    }
    
    ~Freelancer() override {
        cout << "  [Freelancer] Удален фрилансер\n";
    }
    
    double calculateSalary() const override {
        return projectPayment;
    }
    
    string getType() const override {
        return "Фрилансер (оплата за проект)";
    }
};

// Система расчета зарплаты
class PayrollSystem {
private:
    vector<unique_ptr<Employee>> employees;
    
    int getUniqueId() {
        int suggested = Employee::suggestNewId();
        cout << "Предлагаемый ID: " << suggested << "\n";
        int id = getValidatedInt("Введите ID: ", 1, 9999);
        
        while (Employee::isIdUsed(id)) {
            cout << "Ошибка! ID " << id << " уже занят.\n";
            cout << "Ближайший свободный: " << Employee::suggestNewId() << "\n";
            id = getValidatedInt("Введите другой ID: ", 1, 9999);
        }
        return id;
    }
    
public:
    void addEmployee() {
        cout << "\nТип сотрудника:\n";
        cout << "1. Штатный (оклад + премия)\n";
        cout << "2. Почасовой (ставка × часы)\n";
        cout << "3. Менеджер (оклад + % от продаж)\n";
        cout << "4. Фрилансер (оплата за проект)\n";
        int type = getValidatedInt("Выберите тип: ", 1, 4);
        
        string name = getValidatedString("Имя: ");
        int id = getUniqueId();
        
        switch (type) {
            case 1: {
                double salary = getValidatedDouble("Оклад: ", 0.0, 10000000.0);
                double bonus = getValidatedDouble("Фиксированная премия: ", 0.0, 10000000.0);
                auto emp = make_unique<SalariedEmployee>(name, id, salary, bonus);
                cout << "Сотрудник добавлен! Расчетная зарплата: " 
                     << fixed << setprecision(2) << emp->calculateSalary() << " руб.\n";
                employees.push_back(move(emp));
                break;
            }
            case 2: {
                double hRate = getValidatedDouble("Ставка в час: ", 0.0, 100000.0);
                int hours = getValidatedInt("Отработанные часы: ", 0, 744);
                auto emp = make_unique<HourlyEmployee>(name, id, hRate, hours);
                cout << "Сотрудник добавлен! Расчетная зарплата: " 
                     << fixed << setprecision(2) << emp->calculateSalary() << " руб.\n";
                employees.push_back(move(emp));
                break;
            }
            case 3: {
                double salary = getValidatedDouble("Оклад: ", 0.0, 10000000.0);
                double percent = getValidatedDouble("Процент от продаж: ", 0.0, 100.0);
                double sales = getValidatedDouble("Продажи отдела: ", 0.0, 100000000.0);
                auto emp = make_unique<Manager>(name, id, salary, percent, sales);
                cout << "Сотрудник добавлен! Расчетная зарплата: " 
                     << fixed << setprecision(2) << emp->calculateSalary() << " руб.\n";
                employees.push_back(move(emp));
                break;
            }
            case 4: {
                double payment = getValidatedDouble("Оплата за проект: ", 0.0, 10000000.0);
                auto emp = make_unique<Freelancer>(name, id, payment);
                cout << "Сотрудник добавлен! Расчетная зарплата: " 
                     << fixed << setprecision(2) << emp->calculateSalary() << " руб.\n";
                employees.push_back(move(emp));
                break;
            }
        }
        cout << "Всего сотрудников: " << Employee::getTotalEmployees() << "\n";
    }
    
    void calculateAllSalaries() const {
        if (employees.empty()) {
            cout << "\nНет сотрудников для расчета.\n";
            return;
        }
        cout << "\n=== РАСЧЕТ ЗАРПЛАТЫ ВСЕМ СОТРУДНИКАМ ===\n";
        cout << string(70, '-') << endl;
        double total = 0.0;
        for (const auto& emp : employees) {
            double salary = emp->calculateSalary();
            cout << "ID: " << emp->getId() << " | " << emp->getName() 
                 << " | " << emp->getType() 
                 << " | Зарплата: " << fixed << setprecision(2) << salary << " руб.\n";
            total += salary;
        }
        cout << string(70, '-') << endl;
        cout << "Общий фонд оплаты труда: " << fixed << setprecision(2) << total << " руб.\n";
        cout << "Всего сотрудников: " << Employee::getTotalEmployees() << "\n";
    }
    
    void findEmployee() const {
        if (employees.empty()) {
            cout << "\nНет сотрудников для поиска.\n";
            return;
        }
        int id = getValidatedInt("Введите ID сотрудника: ", 1, 9999);
        
        
        vector<const Employee*> found;
        for (const auto& emp : employees) {
            if (emp->getId() == id) found.push_back(emp.get());
        }
        
        if (found.empty()) {
            cout << "Сотрудник с ID " << id << " не найден.\n";
            return;
        }
        
        if (found.size() == 1) {
            cout << "\nНайден сотрудник:\n";
        } else {
            cout << "\nНайдено " << found.size() << " сотрудников:\n";
        }
        
        for (const auto& emp : found) {
            cout << "ID: " << emp->getId() << " | " << emp->getName() 
                 << " | " << emp->getType() 
                 << " | Зарплата: " << fixed << setprecision(2) << emp->calculateSalary() << " руб.\n";
        }
    }
    
    void showPayroll() const {
        if (employees.empty()) {
            cout << "\nВедомость пуста.\n";
            return;
        }
        cout << "\n=== ПЛАТЕЖНАЯ ВЕДОМОСТЬ ===\n";
        cout << string(80, '-') << endl;
        cout << std::left << std::setw(6) << "ID" << std::setw(25) << "Имя" 
             << std::setw(30) << "Тип" << std::right << std::setw(15) << "Сумма" << "\n";
        cout << string(80, '-') << endl;
        double total = 0.0;
        for (const auto& emp : employees) {
            double salary = emp->calculateSalary();
            cout << std::left << std::setw(6) << emp->getId() 
                 << std::setw(25) << emp->getName() 
                 << std::setw(30) << emp->getType() 
                 << std::right << std::setw(15) << fixed << setprecision(2) << salary << "\n";
            total += salary;
        }
        cout << string(80, '-') << endl;
        cout << std::right << std::setw(61) << "ИТОГО: " << std::setw(15) << total << "\n";
    }
    
    void saveToFile() const {
        ofstream file("payroll_data.txt");
        if (!file) {
            cout << "Ошибка открытия файла для записи.\n";
            return;
        }
        for (const auto& emp : employees) {
            file << emp->getId() << "|" << emp->getName() << "|" 
                 << emp->getType() << "|" << fixed << setprecision(2) << emp->calculateSalary() << "\n";
        }
        file.close();
        cout << "Данные сохранены в payroll_data.txt\n";
    }
    
    void run() {
        int choice;
        do {
            cout << "\n=== РАСЧЕТ ЗАРПЛАТЫ ===\n";
            cout << "1. Добавить сотрудника\n";
            cout << "2. Рассчитать зарплату всем\n";
            cout << "3. Найти сотрудника\n";
            cout << "4. Показать ведомость\n";
            cout << "5. Сохранить данные\n";
            cout << "0. Вернуться в главное меню\n";
            choice = getValidatedInt("Выберите действие: ", 0, 5);
            
            switch (choice) {
                case 1: addEmployee(); break;
                case 2: calculateAllSalaries(); break;
                case 3: findEmployee(); break;
                case 4: showPayroll(); break;
                case 5: saveToFile(); break;
            }
        } while (choice != 0);
    }
};

void task2_PayrollSystem() {
    PayrollSystem system;
    system.run();
}
// ============================================
// ЗАДАЧА 3: СИМУЛЯТОР БАНКОМАТА
// ============================================

// Пользовательские исключения
class InvalidPinException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Неверный PIN-код!";
    }
};

class InsufficientFundsException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Недостаточно средств!";
    }
};

class CardBlockedException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Карта заблокирована! Свяжитесь с банком.";
    }
};

class CardNotFoundException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Карта с таким номером не найдена!";
    }
};

// Базовый класс BankCard
class BankCard {
protected:
    string cardNumber;
    int pin;
    string ownerName;
    double balance;
    bool blocked;
    int failedAttempts;
    
public:
    BankCard(const string& number, int p, const string& owner, double initialBalance)
        : cardNumber(number), pin(p), ownerName(owner), balance(initialBalance), blocked(false), failedAttempts(0) {
        cout << "[BankCard] Создана карта " << cardNumber << " владелец: " << ownerName << "\n";
    }
    
    virtual ~BankCard() {
        cout << "[BankCard] Удалена карта " << cardNumber << "\n";
    }
    
    virtual bool withdraw(double amount) = 0;
    virtual void deposit(double amount) = 0;
    virtual string getBonusInfo() const = 0;
    virtual string getCardType() const = 0;
    
    bool checkPin(int enteredPin) {
        if (blocked) throw CardBlockedException();
        if (enteredPin != pin) {
            failedAttempts++;
            if (failedAttempts >= 3) {
                blocked = true;
                throw CardBlockedException();
            }
            throw InvalidPinException();
        }
        failedAttempts = 0;
        return true;
    }
    
    double getBalance() const { return balance; }
    string getOwner() const { return ownerName; }
    string getNumber() const { return cardNumber; }
    bool isBlocked() const { return blocked; }
    int getPin() const { return pin; }  // для отладки/тестирования
};

// Дебетовая карта
class DebitCard : public BankCard {
public:
    DebitCard(const string& number, int p, const string& owner, double initialBalance)
        : BankCard(number, p, owner, initialBalance) {
        cout << "  [DebitCard] Дебетовая карта, баланс: " << balance << " руб.\n";
    }
    
    ~DebitCard() override {
        cout << "  [DebitCard] Удалена дебетовая карта\n";
    }
    
    bool withdraw(double amount) override {
        if (amount > balance) {
            throw InsufficientFundsException();
        }
        balance -= amount;
        return true;
    }
    
    void deposit(double amount) override {
        balance += amount;
    }
    
    string getBonusInfo() const override {
        return "Дебетовая карта: бонусы не начисляются";
    }
    
    string getCardType() const override {
        return "Дебетовая";
    }
};

// Кредитная карта
class CreditCard : public BankCard {
private:
    double creditLimit;
    double usedCredit;
    
public:
    CreditCard(const string& number, int p, const string& owner, double initialBalance, double limit)
        : BankCard(number, p, owner, initialBalance), creditLimit(limit), usedCredit(0.0) {
        cout << "  [CreditCard] Кредитная карта, лимит: " << creditLimit << " руб.\n";
    }
    
    ~CreditCard() override {
        cout << "  [CreditCard] Удалена кредитная карта\n";
    }
    
    bool withdraw(double amount) override {
        double available = balance + (creditLimit - usedCredit);
        if (amount > available) {
            throw InsufficientFundsException();
        }
        if (amount > balance) {
            double creditUsed = amount - balance;
            balance = 0;
            usedCredit += creditUsed;
        } else {
            balance -= amount;
        }
        return true;
    }
    
    void deposit(double amount) override {
        if (usedCredit > 0) {
            if (amount >= usedCredit) {
                balance += (amount - usedCredit);
                usedCredit = 0;
            } else {
                usedCredit -= amount;
            }
        } else {
            balance += amount;
        }
    }
    
    string getBonusInfo() const override {
        ostringstream oss;
        oss << "Кредитная карта | Лимит: " << fixed << setprecision(2) << creditLimit
            << " | Использовано: " << usedCredit << " | Доступно: " << (creditLimit - usedCredit);
        return oss.str();
    }
    
    string getCardType() const override {
        return "Кредитная";
    }
};

// Бонусная карта
class BonusCard : public BankCard {
private:
    double bonusPoints;
    
public:
    BonusCard(const string& number, int p, const string& owner, double initialBalance, double initialBonus)
        : BankCard(number, p, owner, initialBalance), bonusPoints(initialBonus) {
        cout << "  [BonusCard] Бонусная карта, бонусов: " << bonusPoints << "\n";
    }
    
    ~BonusCard() override {
        cout << "  [BonusCard] Удалена бонусная карта\n";
    }
    
    bool withdraw(double amount) override {
        // Можно списать до 50% суммы бонусами
        double maxBonusUse = amount * 0.5;
        double bonusToUse = (bonusPoints < maxBonusUse) ? bonusPoints : maxBonusUse;
        double cashNeeded = amount - bonusToUse;
        
        if (cashNeeded > balance) {
            throw InsufficientFundsException();
        }
        
        balance -= cashNeeded;
        bonusPoints -= bonusToUse;
        return true;
    }
    
    void deposit(double amount) override {
        balance += amount;
        bonusPoints += amount * 0.01;  // 1% кэшбэк бонусами
    }
    
    string getBonusInfo() const override {
        ostringstream oss;
        oss << "Бонусная карта | Бонусов: " << fixed << setprecision(2) << bonusPoints;
        return oss.str();
    }
    
    string getCardType() const override {
        return "Бонусная";
    }
};

// RAII: блокировка карты при работе с ATM
class CardSession {
private:
    BankCard* card;
    
public:
    CardSession(BankCard* c) : card(c) {}
    
    ~CardSession() {
        cout << "[CardSession] Карта извлечена из банкомата.\n";
    }
    
    BankCard* getCard() const { return card; }
};

// Класс ATM
class ATM {
private:
    vector<unique_ptr<BankCard>> cards;
    
    BankCard* findCard(const string& number) {
        for (auto& card : cards) {
            if (card->getNumber() == number) {
                return card.get();
            }
        }
        return nullptr;
    }
    
    void createCard() {
        cout << "\n=== СОЗДАНИЕ НОВОЙ КАРТЫ ===\n";
        cout << "Тип карты:\n";
        cout << "1. Дебетовая\n";
        cout << "2. Кредитная\n";
        cout << "3. Бонусная\n";
        int type = getValidatedInt("Выберите тип: ", 1, 3);
        
        string number = getValidatedString("Номер карты (16 цифр): ");
        while (number.length() != 16 || !all_of(number.begin(), number.end(), ::isdigit)) {
            cout << "Ошибка! Номер карты должен содержать ровно 16 цифр.\n";
            number = getValidatedString("Номер карты (16 цифр): ");
        }
        
        // Форматируем номер: 1234 5678 9012 3456
        string formattedNumber = number.substr(0, 4) + " " + number.substr(4, 4) + " " + 
                                 number.substr(8, 4) + " " + number.substr(12, 4);
        
        if (findCard(formattedNumber)) {
            cout << "Ошибка! Карта с таким номером уже существует.\n";
            return;
        }
        
        int pin = getValidatedPin("PIN-код (4 цифры): ");
        string owner = getValidatedString("Имя владельца: ");
        double balance = getValidatedDouble("Начальный баланс: ", 0.0, 10000000.0);
        
        switch (type) {
            case 1: {
                cards.push_back(make_unique<DebitCard>(formattedNumber, pin, owner, balance));
                break;
            }
            case 2: {
                double limit = getValidatedDouble("Кредитный лимит: ", 0.0, 10000000.0);
                cards.push_back(make_unique<CreditCard>(formattedNumber, pin, owner, balance, limit));
                break;
            }
            case 3: {
                double bonus = getValidatedDouble("Начальные бонусы: ", 0.0, 1000000.0);
                cards.push_back(make_unique<BonusCard>(formattedNumber, pin, owner, balance, bonus));
                break;
            }
        }
        
        cout << "\nКарта успешно создана!\n";
        cout << "Номер: " << formattedNumber << "\n";
        cout << "PIN: " << pin << " (запомните его!)\n";
        cout << "Тип: " << cards.back()->getCardType() << "\n";
        cout << "Всего карт в системе: " << cards.size() << "\n";
    }
    
    void showAllCards() const {
        if (cards.empty()) {
            cout << "\nВ системе нет карт. Сначала создайте карту.\n";
            return;
        }
        cout << "\n=== ДОСТУПНЫЕ КАРТЫ ===\n";
        cout << string(60, '-') << endl;
        for (size_t i = 0; i < cards.size(); ++i) {
            cout << (i + 1) << ". " << cards[i]->getNumber() 
                 << " | " << cards[i]->getCardType()
                 << " | " << cards[i]->getOwner()
                 << (cards[i]->isBlocked() ? " | ЗАБЛОКИРОВАНА" : "") << "\n";
        }
        cout << string(60, '-') << endl;
    }
    
    void useCard() {
    if (cards.empty()) {
        cout << "\nВ системе нет карт. Сначала создайте карту.\n";
        return;
    }
    
    showAllCards();
    
    int index = getValidatedInt("Введите номер карты из списка (или 0 для выхода): ", 0, static_cast<int>(cards.size())) - 1;
    if (index < 0) return;
    
    BankCard* card = cards[static_cast<size_t>(index)].get();
    
    if (card->isBlocked()) {
        cout << "Ошибка: эта карта заблокирована.\n";
        return;
    }
    
    // Проверка PIN
    int attempts = 0;
    while (attempts < 3) {
        try {
            int pin = getValidatedPin("Введите PIN (4 цифры): ");
            card->checkPin(pin);
            break;
        } catch (const InvalidPinException& e) {
            cout << "Ошибка: " << e.what() << " Осталось попыток: " << (2 - attempts) << "\n";
            attempts++;
            if (attempts >= 3) {
                cout << "Карта заблокирована! Обратитесь в банк.\n";
                return;
            }
        } catch (const CardBlockedException& e) {
            cout << "Ошибка: " << e.what() << "\n";
            return;
        }
    }
    
    cout << "\nДобро пожаловать, " << card->getOwner() << "!\n";
    
    // RAII: карта "вставлена"
    CardSession session(card);
    
    int choice;
    do {
        cout << "\nБаланс: " << fixed << setprecision(2) << card->getBalance() << " руб.\n";
        cout << "Тип карты: " << card->getCardType() << "\n";
        cout << card->getBonusInfo() << "\n";
        cout << "\n1. Снять наличные\n";
        cout << "2. Внести наличные\n";
        cout << "3. Показать баланс\n";
        cout << "4. Показать бонусы/кредитный лимит\n";
        cout << "5. Извлечь карту\n";
        choice = getValidatedInt("Выберите действие: ", 1, 5);
        
        try {
            switch (choice) {
                case 1: {
                    double amount = getValidatedDouble("Сумма для снятия: ", 1.0, 1000000.0);
                    if (card->withdraw(amount)) {
                        cout << "Операция выполнена! Получите деньги.\n";
                        cout << "Новый баланс: " << fixed << setprecision(2) << card->getBalance() << " руб.\n";
                    }
                    break;
                }
                case 2: {
                    double amount = getValidatedDouble("Сумма для внесения: ", 1.0, 1000000.0);
                    card->deposit(amount);
                    cout << "Счет пополнен. Новый баланс: " << fixed << setprecision(2) << card->getBalance() << " руб.\n";
                    break;
                }
                case 3: {
                    cout << "\n=== ПЕЧАТЬ ЧЕКА ===\n";
                    cout << "Владелец: " << card->getOwner() << "\n";
                    cout << "Карта: " << card->getNumber() << "\n";
                    cout << "Тип: " << card->getCardType() << "\n";
                    cout << "Баланс: " << fixed << setprecision(2) << card->getBalance() << " руб.\n";
                    cout << card->getBonusInfo() << "\n";
                    break;
                }
                case 4: {
                    cout << "\n" << card->getBonusInfo() << "\n";
                    break;
                }
            }
        } catch (const InsufficientFundsException& e) {
            cout << "Ошибка: " << e.what() << "\n";
        }
        
    } while (choice != 5);
    
    cout << "Спасибо за использование банкомата!\n";
}
    
public:
    void run() {
        int choice;
        do {
            cout << "\n=== БАНКОМАТ \"Надежный\" ===\n";
            cout << "Всего карт в системе: " << cards.size() << "\n";
            cout << "\n1. Создать новую карту\n";
            cout << "2. Показать все карты\n";
            cout << "3. Вставить карту (работа с банкоматом)\n";
            cout << "0. Вернуться в главное меню\n";
            choice = getValidatedInt("Выберите действие: ", 0, 3);
            
            switch (choice) {
                case 1: createCard(); break;
                case 2: showAllCards(); break;
                case 3: useCard(); break;
            }
        } while (choice != 0);
    }
};

void task3_ATM() {
    ATM atm;
    atm.run();
}

// ============================================
// ГЛАВНОЕ МЕНЮ
// ============================================
void displayMainMenu() {
    cout << "\n=================================" << endl;
    cout << "    ЛАБОРАТОРНАЯ РАБОТА 2" << endl;
    cout << "=================================" << endl;
    cout << "1. Система бронирования билетов (Кино/Авиа/ЖД)" << endl;
    cout << "2. Система расчета зарплаты" << endl;
    cout << "3. Симулятор банкомата" << endl;
    cout << "0. Выход" << endl;
    cout << "=================================" << endl;
}

int main() {
    setlocale(LC_ALL, "");
    srand(static_cast<unsigned int>(time(nullptr)));
    
    int choice;
    do {
        displayMainMenu();
        choice = getValidatedInt("Выберите задачу: ", 0, 3);
        
        switch (choice) {
            case 1:
                executeTask(task1_BookingSystem, "Система бронирования билетов");
                break;
            case 2:
                executeTask(task2_PayrollSystem, "Система расчета зарплаты");
                break;
            case 3:
                executeTask(task3_ATM, "Симулятор банкомата");
                break;
            case 0:
                cout << "\nСпасибо за использование программы! До свидания!\n";
                break;
        }
    } while (choice != 0);
    
    return 0;
}