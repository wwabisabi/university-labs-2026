#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
#include <cmath>
#include <thread>
#include <chrono>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <atomic>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <cctype>
#include <clocale>

using std::cin;
using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::vector;
using std::map;
using std::atomic;
using std::istringstream;
using std::stringstream;
using std::numeric_limits;
using std::streamsize;
using std::fixed;
using std::setprecision;
using std::to_string;
using std::max;
using std::abs;

// глобальные переменные для таймера
namespace {
    atomic<char> pressedKey(0);
    atomic<bool> keyAvailable(false);
    atomic<bool> listenerRunning(false);
    struct termios old_termios;
}

// функции для работы с терминалом Linux

void restoreTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

void* keyboardListener(void* /*arg*/) {
    tcgetattr(STDIN_FILENO, &old_termios);
    
    struct termios newt = old_termios;
    newt.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    listenerRunning = true;
    
    char ch;
    while (listenerRunning) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        
        int ret = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        
        if (ret > 0 && FD_ISSET(STDIN_FILENO, &fds)) {
            if (read(STDIN_FILENO, &ch, 1) > 0) {
                pressedKey = ch;
                keyAvailable = true;
            }
        }
    }
    
    restoreTerminal();
    return NULL;
}

void displayProgressBar(int current, int total, int width = 30) {
    int progress = (current * width) / total;
    cout << "[";
    for (int i = 0; i < width; i++) {
        if (i < progress) cout << "=";
        else if (i == progress) cout << ">";
        else cout << " ";
    }
    cout << "] " << (current * 100 / total) << "%";
}


// прототипы функций задач

void task1_PasswordGenerator();
void task2_NumberConverter();
void task3_ATM_Simulator();
void task4_RockPaperScissors();
void task5_TextAnalyzer();
void task6_CountdownTimer();

// вспомагательные функции

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
        
        if (ss.eof() && abs(temp - floor(temp)) < 1e-9 && temp >= min && temp <= max) {
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

char getValidatedChar(const string& prompt, const string& validChars) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (input.empty()) {
            cout << "Ошибка! Введите один из символов: ";
            for (char c : validChars) cout << c << " ";
            cout << ": ";
            continue;
        }
        
        char ch = static_cast<char>(tolower(input[0]));
        if (validChars.find(ch) != string::npos) {
            return ch;
        }
        
        cout << "Ошибка! Введите один из символов: ";
        for (char c : validChars) cout << c << " ";
        cout << ": ";
    }
}

string getValidatedNumberString(const string& prompt, int base) {
    string validChars;
    if (base <= 10) {
        for (int i = 0; i < base; i++) {
            validChars += static_cast<char>('0' + i);
        }
    } else {
        for (int i = 0; i < 10; i++) {
            validChars += static_cast<char>('0' + i);
        }
        for (int i = 0; i < base - 10; i++) {
            validChars += static_cast<char>('A' + i);
            validChars += static_cast<char>('a' + i);
        }
    }
    
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (input.empty()) {
            cout << "Ошибка! Число не может быть пустым.\n";
            continue;
        }
        
        bool isValid = true;
        for (char c : input) {
            if (validChars.find(c) == string::npos) {
                isValid = false;
                break;
            }
        }
        
        if (isValid) {
            return input;
        }
        
        cout << "Ошибка! Число содержит недопустимые символы для системы счисления " << base << ".\n";
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


// ЗАДАЧА 1: Генератор случайных паролей

void task1_PasswordGenerator() {
    int length = getValidatedInt("Введите длину пароля (6-32): ", 6, 32);
    
    cout << "\nВыберите типы символов для пароля:\n";
    char useDigits = getValidatedChar("Использовать цифры? (y/n): ", "yn");
    char useUppercase = getValidatedChar("Использовать прописные буквы? (y/n): ", "yn"); 
    char useLowercase = getValidatedChar("Использовать строчные буквы? (y/n): ", "yn");
    char useSpecial = getValidatedChar("Использовать спецсимволы? (y/n): ", "yn");
    
    if (useDigits == 'n' && useUppercase == 'n' && 
        useLowercase == 'n' && useSpecial == 'n') {
        cout << "Ошибка! Должен быть выбран хотя бы один тип символов.\n";
        cout << "Автоматически добавлены строчные буквы.\n";
        useLowercase = 'y';
    }
    
    string charset = "";
    if (useDigits == 'y') charset += "0123456789";
    if (useUppercase == 'y') charset += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (useLowercase == 'y') charset += "abcdefghijklmnopqrstuvwxyz";
    if (useSpecial == 'y') charset += "!@#$%^&*()-_=+[]{}|;:,.<>?";
    
    int numPasswords = getValidatedInt("\nСколько паролей сгенерировать? (1-10): ", 1, 10);
    
    cout << "\nСгенерированные пароли:\n";
    cout << string(50, '-') << endl;
    
    string::size_type charsetLen = charset.length();
    for (int p = 0; p < numPasswords; p++) {
        string password = "";
        for (int i = 0; i < length; i++) {
            int randomIndex = static_cast<int>(static_cast<string::size_type>(rand()) % charsetLen);
            password += charset[static_cast<string::size_type>(randomIndex)];
        }
        
        string strength;
        double entropy = static_cast<double>(length) * log2(static_cast<double>(charset.length()));
        
        if (entropy < 40.0) {
            strength = "СЛАБЫЙ";
        } else if (entropy < 60.0) {
            strength = "СРЕДНИЙ";
        } else {
            strength = "СИЛЬНЫЙ";
        }
        
        cout << "Пароль " << (p + 1) << ": " << password 
             << " [" << strength << "]" << endl;
    }
    cout << string(50, '-') << endl;
}

// ЗАДАЧА 2: Конвертер систем счисления

int charToValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

char valueToChar(int value) {
    if (value >= 0 && value <= 9) return static_cast<char>('0' + value);
    if (value >= 10 && value <= 15) return static_cast<char>('A' + (value - 10));
    return '0';
}

unsigned long long toDecimal(const string& number, int sourceBase) {
    unsigned long long result = 0;
    unsigned long long base = static_cast<unsigned long long>(sourceBase);
    for (char c : number) {
        result = result * base + static_cast<unsigned long long>(charToValue(c));
    }
    return result;
}

string fromDecimal(unsigned long long decimal, int targetBase) {
    if (decimal == 0) return "0";
    
    unsigned long long base = static_cast<unsigned long long>(targetBase);
    string result = "";
    while (decimal > 0) {
        int remainder = static_cast<int>(decimal % base);
        result = valueToChar(remainder) + result;
        decimal /= base;
    }
    return result;
}

void task2_NumberConverter() {
    cout << "КОНВЕРТЕР СИСТЕМ СЧИСЛЕНИЯ\n";
    cout << "===========================\n";
    
    int sourceBase = getValidatedInt("Введите исходную систему счисления (2-16): ", 2, 16);
    string sourceNumber = getValidatedNumberString(
        "Введите число в " + to_string(sourceBase) + "-й системе счисления: ", 
        sourceBase
    );
    
    int targetBase;
    while (true) {
        targetBase = getValidatedInt("Введите целевую систему счисления (2-16): ", 2, 16);
        if (targetBase == sourceBase) {
            cout << "Целевая система счисления должна отличаться от исходной!\n";
        } else {
            break;
        }
    }
    
    unsigned long long decimalValue = toDecimal(sourceNumber, sourceBase);
    string result = fromDecimal(decimalValue, targetBase);
    
    string formattedSource = sourceNumber;
    for (char& c : formattedSource) {
        c = static_cast<char>(toupper(c));
    }
    
    cout << "\nРЕЗУЛЬТАТ КОНВЕРТАЦИИ:\n";
    cout << string(50, '-') << endl;
    cout << "Число " << formattedSource << " в системе " << sourceBase 
         << " = " << result << " в системе " << targetBase << endl;
    cout << string(50, '-') << endl;
    
    cout << "\nДополнительная информация:\n";
    cout << "Десятичное представление: " << decimalValue << endl;
    cout << "Двоичное представление: " << fromDecimal(decimalValue, 2) << endl;
    cout << "Восьмеричное представление: " << fromDecimal(decimalValue, 8) << endl;
    cout << "Шестнадцатеричное представление: " << fromDecimal(decimalValue, 16) << endl;
}

// ЗАДАЧА 3: Симулятор банкомата

void task3_ATM_Simulator() {
    const int MAX_WITHDRAWAL = 500000;
    const int MAX_OPERATIONS = 10;
    const int MAX_DEPOSIT = 100000000;
    int balance = 1000000;
    int operationCount = 0;
    
    cout << fixed;
    cout.precision(2);
    
    cout << "ДОБРО ПОЖАЛОВАТЬ В БАНКОМАТ\n";
    cout << "============================\n";
    cout << "Начальный баланс: " << (balance / 100.0) << " руб.\n";
    
    while (operationCount < MAX_OPERATIONS) {
        cout << "\nМЕНЮ ОПЕРАЦИЙ:\n";
        cout << "1. Проверка баланса\n";
        cout << "2. Снятие наличных\n";
        cout << "3. Пополнение счета\n";
        cout << "4. Выход\n";
        cout << "Осталось операций: " << (MAX_OPERATIONS - operationCount) << endl;
        
        int choice = getValidatedInt("Выберите операцию: ", 1, 4);
        
        switch (choice) {
            case 1: {
                cout << "\n=== ПРОВЕРКА БАЛАНСА ===\n";
                cout << "Текущий баланс: " << (balance / 100.0) << " руб.\n";
                operationCount++;
                break;
            }
            case 2: {
                cout << "\n=== СНЯТИЕ НАЛИЧНЫХ ===\n";
                cout << "Максимальная сумма снятия: " << (MAX_WITHDRAWAL / 100.0) << " руб.\n";
                cout << "Доступно на счете: " << (balance / 100.0) << " руб.\n";
                
                double amount = getValidatedDouble("Введите сумму для снятия: ", 0.01, balance / 100.0);
                int amountKopecks = static_cast<int>(amount * 100.0 + 0.5);
                
                if (amountKopecks > MAX_WITHDRAWAL) {
                    cout << "Ошибка! Превышен лимит снятия (" 
                         << (MAX_WITHDRAWAL / 100.0) << " руб.)\n";
                    break;
                }
                if (amountKopecks > balance) {
                    cout << "Ошибка! Недостаточно средств. Доступно: " 
                         << (balance / 100.0) << " руб.\n";
                    break;
                }
                
                balance -= amountKopecks;
                cout << "Успешно снято: " << (amountKopecks / 100.0) << " руб.\n";
                cout << "Остаток на счете: " << (balance / 100.0) << " руб.\n";
                operationCount++;
                break;
            }
            case 3: {
                cout << "\n=== ПОПОЛНЕНИЕ СЧЕТА ===\n";
                cout << "Максимальная сумма пополнения: " << (MAX_DEPOSIT / 100.0) << " руб.\n";
                cout << "Текущий баланс: " << (balance / 100.0) << " руб.\n";
                
                double amount = getValidatedDouble("Введите сумму пополнения: ", 0.01, MAX_DEPOSIT / 100.0);
                int amountKopecks = static_cast<int>(amount * 100.0 + 0.5);
                
                balance += amountKopecks;
                cout << "Счет успешно пополнен на: " << (amountKopecks / 100.0) << " руб.\n";
                cout << "Текущий баланс: " << (balance / 100.0) << " руб.\n";
                operationCount++;
                break;
            }
            case 4: {
                cout << "\nСпасибо за использование банкомата!\n";
                cout << "Итоговый баланс: " << (balance / 100.0) << " руб.\n";
                cout << "Выполнено операций: " << operationCount << endl;
                return;
            }
        }
        
        if (operationCount >= MAX_OPERATIONS) {
            cout << "\nДостигнут лимит операций за сеанс (" << MAX_OPERATIONS << ").\n";
            cout << "Сеанс завершен. Итоговый баланс: " << (balance / 100.0) << " руб.\n";
        }
    }
}

// ЗАДАЧА 4: Игра "Камень, ножницы, бумага"

int getComputerChoice(int difficulty, int playerHistory[], int historySize) {
    switch (difficulty) {
        case 1: {
            return (rand() % 3) + 1;
        }
        case 2: {
            if (historySize < 3) {
                return (rand() % 3) + 1;
            }
            int counts[4] = {0, 0, 0, 0};
            for (int i = max(0, historySize - 5); i < historySize; i++) {
                counts[playerHistory[i]]++;
            }
            int mostFrequent = 1;
            if (counts[2] > counts[mostFrequent]) mostFrequent = 2;
            if (counts[3] > counts[mostFrequent]) mostFrequent = 3;
            
            if (mostFrequent == 1) return 2;
            if (mostFrequent == 2) return 3;
            return 1;
        }
        case 3: {
            if (historySize < 4) {
                return (rand() % 3) + 1;
            }
            int pattern[2] = {playerHistory[historySize - 2], playerHistory[historySize - 1]};
            int predictedNext = -1;
            
            for (int i = 0; i < historySize - 2; i++) {
                if (playerHistory[i] == pattern[0] && playerHistory[i + 1] == pattern[1]) {
                    if (i + 2 < historySize) {
                        predictedNext = playerHistory[i + 2];
                    }
                }
            }
            
            if (predictedNext == -1) {
                int counts[4] = {0, 0, 0, 0};
                for (int i = max(0, historySize - 5); i < historySize; i++) {
                    counts[playerHistory[i]]++;
                }
                int mostFrequent = 1;
                if (counts[2] > counts[mostFrequent]) mostFrequent = 2;
                if (counts[3] > counts[mostFrequent]) mostFrequent = 3;
                predictedNext = mostFrequent;
            }
            
            if (predictedNext == 1) return 2;
            if (predictedNext == 2) return 3;
            return 1;
        }
    }
    return 1;
}

string choiceToString(int choice) {
    switch (choice) {
        case 1: return "Камень";
        case 2: return "Ножницы";
        case 3: return "Бумага";
        default: return "Неизвестно";
    }
}

void task4_RockPaperScissors() {
    cout << "ИГРА \"КАМЕНЬ, НОЖНИЦЫ, БУМАГА\"\n";
    cout << "================================\n";
    
    cout << "\nВыберите уровень сложности:\n";
    cout << "1. Легкий (компьютер играет случайно)\n";
    cout << "2. Средний (компьютер анализирует ваши ходы)\n";
    cout << "3. Сложный (компьютер пытается предугадать)\n";
    
    int difficulty = getValidatedInt("Ваш выбор: ", 1, 3);
    
    int playerWins = 0, computerWins = 0, ties = 0;
    int roundsPlayed = 0;
    const int MAX_HISTORY = 100;
    int playerHistory[MAX_HISTORY] = {0};
    
    cout << "\nНачинаем игру!\n";
    cout << "1 - Камень, 2 - Ножницы, 3 - Бумага, 0 - Закончить игру\n\n";
    
    while (true) {
        int playerChoice = getValidatedInt("Ваш выбор: ", 0, 3);
        
        if (playerChoice == 0) {
            break;
        }
        
        int computerChoice = getComputerChoice(difficulty, playerHistory, roundsPlayed);
        
        cout << "Вы выбрали: " << choiceToString(playerChoice) << endl;
        cout << "Компьютер выбрал: " << choiceToString(computerChoice) << endl;
        
        if (playerChoice == computerChoice) {
            cout << "Ничья!\n";
            ties++;
        } else if ((playerChoice == 1 && computerChoice == 2) ||
                   (playerChoice == 2 && computerChoice == 3) ||
                   (playerChoice == 3 && computerChoice == 1)) {
            cout << "Вы выиграли!\n";
            playerWins++;
        } else {
            cout << "Компьютер выиграл!\n";
            computerWins++;
        }
        
        if (roundsPlayed < MAX_HISTORY) {
            playerHistory[roundsPlayed] = playerChoice;
        }
        roundsPlayed++;
        
        cout << "Счет: Вы " << playerWins << " : " << computerWins 
             << " Компьютер (Ничьих: " << ties << ")\n\n";
    }
    
    cout << "\n=== СТАТИСТИКА ИГРЫ ===\n";
    cout << "Всего раундов: " << roundsPlayed << endl;
    cout << "Побед: " << playerWins << endl;
    cout << "Поражений: " << computerWins << endl;
    cout << "Ничьих: " << ties << endl;
    
    if (roundsPlayed > 0) {
        cout << "Процент побед: " << fixed << setprecision(1) 
             << (static_cast<double>(playerWins) * 100.0 / static_cast<double>(roundsPlayed)) << "%\n";
        
        if (playerWins > computerWins) {
            cout << "Поздравляем! Вы выиграли игру!\n";
        } else if (playerWins < computerWins) {
            cout << "Компьютер победил. Попробуйте еще раз!\n";
        } else {
            cout << "Ничья в общем зачете!\n";
        }
    }
}

// ЗАДАЧА 5: Анализатор текста

bool isPalindrome(const string& text) {
    string cleanText = "";
    for (char c : text) {
        if (isalnum(c)) {
            cleanText += static_cast<char>(tolower(c));
        }
    }
    
    if (cleanText.empty()) {
        return false;
    }
    
    string::size_type left = 0;
    string::size_type right = cleanText.length() - 1;
    while (left < right) {
        if (cleanText[left] != cleanText[right]) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

bool isLatin(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

void task5_TextAnalyzer() {
    cout << "АНАЛИЗАТОР ТЕКСТА (только латиница)\n";
    cout << "==================================\n";
    
    string text;
    while (true) {
        cout << "Введите текст (до 1000 символов):\n";
        getline(cin, text);
        
        if (!text.empty()) {
            break;
        }
        cout << "Текст не введен. Попробуйте еще раз.\n\n";
    }
    
    if (text.length() > 1000) {
        text = text.substr(0, 1000);
        cout << "Текст обрезан до 1000 символов.\n";
    }
    
    string::size_type charsWithSpaces = text.length();
    string::size_type charsWithoutSpaces = 0;
    for (char c : text) {
        if (!isspace(c)) {
            charsWithoutSpaces++;
        }
    }
    
    vector<string> words;
    string currentWord = "";
    
    for (char c : text) {
        if (isalnum(c)) {
            currentWord += c;
        } else {
            if (!currentWord.empty()) {
                words.push_back(currentWord);
                currentWord = "";
            }
        }
    }
    if (!currentWord.empty()) {
        words.push_back(currentWord);
    }
    
    string::size_type sentences = 0;
    for (char c : text) {
        if (c == '.' || c == '!' || c == '?') {
            sentences++;
        }
    }
    if (sentences == 0 && !text.empty()) {
        sentences = 1;
    }
    
    string longestWord = "", shortestWord = "";
    if (!words.empty()) {
        longestWord = words[0];
        shortestWord = words[0];
        
        for (const string& word : words) {
            if (word.length() > longestWord.length()) {
                longestWord = word;
            }
            if (word.length() < shortestWord.length()) {
                shortestWord = word;
            }
        }
    }
    
    map<char, int> letterFrequency;
    for (char c : text) {
        if (isLatin(c)) {
            letterFrequency[static_cast<char>(tolower(c))]++;
        }
    }
    
    cout << "\n=== СТАТИСТИКА ТЕКСТА ===\n";
    cout << string(40, '-') << endl;
    cout << "Количество символов (с пробелами): " << charsWithSpaces << endl;
    cout << "Количество символов (без пробелов): " << charsWithoutSpaces << endl;
    cout << "Количество слов: " << words.size() << endl;
    cout << "Количество предложений: " << sentences << endl;
    
    if (!words.empty()) {
        cout << "Самое длинное слово: \"" << longestWord 
             << "\" (" << longestWord.length() << " символов)\n";
        cout << "Самое короткое слово: \"" << shortestWord 
             << "\" (" << shortestWord.length() << " символов)\n";
    }
    
    if (!letterFrequency.empty()) {
        cout << "\nЧастота использования букв (только латиница):\n";
        cout << string(40, '-') << endl;
        for (auto& pair : letterFrequency) {
            cout << "'" << pair.first << "': " << pair.second << endl;
        }
    }
    
    cout << "\nПроверка на палиндром: ";
    if (isPalindrome(text)) {
        cout << "ДА, текст является палиндромом!\n";
    } else {
        cout << "НЕТ, текст не является палиндромом.\n";
    }
    
    cout << string(40, '-') << endl;
}

// ЗАДАЧА 6: Таймер обратного отсчета

bool runTimerPhase(int totalSeconds, const string& phaseName, 
                   bool showNotifications, bool& isPaused, bool& quit) {
    bool notified50 = false;
    bool notified25 = false;
    bool notified10 = false;
    
    auto startTime = std::chrono::steady_clock::now();
    std::chrono::seconds pausedDuration(0);
    auto pauseStart = std::chrono::steady_clock::now();
    
    while (!quit) {
        if (keyAvailable) {
            char key = pressedKey;
            keyAvailable = false;
            
            if (key == 'p' || key == 'P') {
                if (!isPaused) {
                    isPaused = true;
                    pauseStart = std::chrono::steady_clock::now();
                    cout << "\n=== ТАЙМЕР НА ПАУЗЕ ===\n";
                    cout << "Нажмите 'p' для продолжения, 'q' для выхода\n";
                } else {
                    isPaused = false;
                    auto pauseEnd = std::chrono::steady_clock::now();
                    pausedDuration += std::chrono::duration_cast<std::chrono::seconds>(pauseEnd - pauseStart);
                    cout << "\nТаймер продолжен!\n";
                }
            } else if (key == 'q' || key == 'Q') {
                quit = true;
                return false;
            }
        }
        
        if (!isPaused && !quit) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime - pausedDuration);
            int remaining = totalSeconds - static_cast<int>(elapsed.count());
            
            if (remaining <= 0) {
                return true;
            }
            
            cout << "\r\033[K" << phaseName << ": " << remaining << " сек ";
            displayProgressBar(totalSeconds - remaining, totalSeconds);
            cout << "  (p-пауза, q-выход)" << flush;
            
            if (showNotifications) {
                int percentRemaining = (remaining * 100) / totalSeconds;
                
                if (!notified50 && percentRemaining <= 50 && percentRemaining > 25) {
                    cout << "\n[УВЕДОМЛЕНИЕ] Осталось 50% времени!\n";
                    notified50 = true;
                }
                if (!notified25 && percentRemaining <= 25 && percentRemaining > 10) {
                    cout << "\n[УВЕДОМЛЕНИЕ] Осталось 25% времени!\n";
                    notified25 = true;
                }
                if (!notified10 && percentRemaining <= 10 && percentRemaining > 0) {
                    cout << "\n[УВЕДОМЛЕНИЕ] Осталось 10% времени! Поторопитесь!\n";
                    notified10 = true;
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    return false;
}

void task6_CountdownTimer() {
    cout << "ТАЙМЕР ОБРАТНОГО ОТСЧЕТА\n";
    cout << "========================\n";
    
    cout << "\nВыберите режим:\n";
    cout << "1. Обычный таймер\n";
    cout << "2. Интервальный таймер\n";
    
    int mode = getValidatedInt("Ваш выбор: ", 1, 2);
    
    int totalSeconds = 0;
    int workTime = 0, restTime = 0, intervals = 0;
    
    if (mode == 1) {
        totalSeconds = getValidatedInt("Установите время в секундах (10-3600): ", 10, 3600);
    } else {
        workTime = getValidatedInt("Время работы (секунд, 5-300): ", 5, 300);
        restTime = getValidatedInt("Время отдыха (секунд, 5-300): ", 5, 300);
        intervals = getValidatedInt("Количество интервалов (1-10): ", 1, 10);
    }
    
    pressedKey = 0;
    keyAvailable = false;
    listenerRunning = true;
    
    pthread_t thread;
    pthread_create(&thread, NULL, keyboardListener, NULL);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    if (mode == 1) {
        cout << "\nТаймер запущен на " << totalSeconds << " секунд.\n";
        cout << "Нажмите 'p' для паузы/продолжения\n";
        cout << "Нажмите 'q' для досрочного завершения\n\n";
        
        bool isPaused = false;
        bool quit = false;
        runTimerPhase(totalSeconds, "Осталось", true, isPaused, quit);
        
        if (quit) {
            cout << "\n\nТаймер остановлен пользователем.\n";
        } else {
            for (int i = 0; i < 3; i++) {
                cout << "\r\033[K";
                if (i % 2 == 0) {
                    cout << "*** ВРЕМЯ ИСТЕКЛО! ***";
                } else {
                    cout << "=====================";
                }
                cout << flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            cout << "\n\n";
            cout << "*****************************\n";
            cout << "*    ВРЕМЯ ИСТЕКЛО!          *\n";
            cout << "*    ТАЙМЕР ЗАВЕРШЕН         *\n";
            cout << "*****************************\n";
            cout << "\a" << flush;
            cout << "BEEP! BEEP! BEEP!\n";
        }
    } else {
        cout << "\nИнтервальный таймер запущен:\n";
        cout << "Работа: " << workTime << " сек, Отдых: " << restTime << " сек\n";
        cout << "Интервалов: " << intervals << "\n";
        cout << "Нажмите 'p' для паузы/продолжения\n";
        cout << "Нажмите 'q' для досрочного завершения\n\n";
        
        bool isPaused = false;
        bool quit = false;
        
        for (int i = 1; i <= intervals && !quit; i++) {
            cout << "\n=== ИНТЕРВАЛ " << i << " - РАБОТА ===\n";
            runTimerPhase(workTime, "Работа", true, isPaused, quit);
            
            if (quit) break;
            
            cout << "\n\a[СИГНАЛ] Время отдыхать!\n";
            
            if (i < intervals) {
                cout << "\n=== ИНТЕРВАЛ " << i << " - ОТДЫХ ===\n";
                runTimerPhase(restTime, "Отдых", false, isPaused, quit);
                
                if (!quit) {
                    cout << "\n\a[СИГНАЛ] Время работать!\n";
                }
            }
        }
        
        if (quit) {
            cout << "\n\nИнтервальный таймер остановлен пользователем.\n";
        } else {
            for (int i = 0; i < 3; i++) {
                cout << "\r\033[K";
                if (i % 2 == 0) {
                    cout << "*** ТРЕНИРОВКА ЗАВЕРШЕНА! ***";
                } else {
                    cout << "============================";
                }
                cout << flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            cout << "\n\n";
            cout << "*****************************\n";
            cout << "*  ИНТЕРВАЛЬНЫЙ ТАЙМЕР       *\n";
            cout << "*  ЗАВЕРШЕН!                 *\n";
            cout << "*****************************\n";
            cout << "\a\a\a" << flush;
            cout << "ТРЕНИРОВКА ОКОНЧЕНА! МОЛОДЕЦ!\n";
        }
    }
    
    listenerRunning = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    pthread_cancel(thread);
    pthread_join(thread, NULL);
    restoreTerminal();
    tcflush(STDIN_FILENO, TCIFLUSH);
}

// ГЛАВНОЕ МЕНЮ

void displayMainMenu() {
    cout << "\n=================================" << endl;
    cout << "    ЛАБОРАТОРНАЯ РАБОТА 1" << endl;
    cout << "=================================" << endl;
    cout << "1. Генератор случайных паролей" << endl;
    cout << "2. Конвертер систем счисления" << endl;
    cout << "3. Симулятор банкомата" << endl;
    cout << "4. Игра \"Камень, ножницы, бумага\"" << endl;
    cout << "5. Анализатор текста" << endl;
    cout << "6. Таймер обратного отсчета с уведомлениями" << endl;
    cout << "0. Выход" << endl;
    cout << "=================================" << endl;
}

int main() {
    setlocale(LC_ALL, "");
    srand(static_cast<unsigned int>(time(nullptr)));
    
    int choice;
    
    do {
        displayMainMenu();
        choice = getValidatedInt("Выберите задачу: ", 0, 6);
        
        switch (choice) {
            case 1:
                executeTask(task1_PasswordGenerator, "Генератор случайных паролей");
                break;
            case 2:
                executeTask(task2_NumberConverter, "Конвертер систем счисления");
                break;
            case 3:
                executeTask(task3_ATM_Simulator, "Симулятор банкомата");
                break;
            case 4:
                executeTask(task4_RockPaperScissors, "Игра \"Камень, ножницы, бумага\"");
                break;
            case 5:
                executeTask(task5_TextAnalyzer, "Анализатор текста");
                break;
            case 6:
                executeTask(task6_CountdownTimer, "Таймер обратного отсчета");
                break;
            case 0:
                cout << "\nСпасибо за использование программы! До свидания!\n";
                break;
        }
    } while (choice != 0);
    
    return 0;
}