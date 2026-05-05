#include "Task2_OnlineCourses.h"
#include <iostream>
#include <limits>
#include <cstdlib>

// ==================== Certificate RAII ====================
Certificate::Certificate(const std::string& student, const std::string& course)
    : student_(student), course_(course), issued_(true) {
    std::cout << "[RAII КОНСТРУКТОР] Сертификат создан для \"" << student_ << "\" по курсу \"" << course_ << "\".\n";
}

Certificate::~Certificate() {
    if (issued_) {
        std::cout << "[RAII ДЕСТРУКТОР] Сертификат выдан и сохранен в системе.\n";
    }
}

void Certificate::print() const {
    std::cout << "\n================================\n";
    std::cout << "        СЕРТИФИКАТ\n";
    std::cout << "================================\n";
    std::cout << "Студент: " << student_ << "\n";
    std::cout << "Курс: " << course_ << "\n";
    std::cout << "Статус: УСПЕШНО ЗАВЕРШЕН\n";
    std::cout << "================================\n\n";
}

// ==================== Lesson ====================
int Lesson::nextId_ = 1;

void Lesson::complete() {
    completed_ = true;
    std::cout << "  [OK] Урок \"" << title_ << "\" пройден.\n";
}

// ==================== VideoLesson ====================
VideoLesson::VideoLesson(const std::string& title) {
    id_ = nextId_++;
    title_ = title;
    completed_ = false;
}
std::string VideoLesson::getType() const { return "Видео-лекция"; }
double VideoLesson::getWeight() const { return 1.0; }

// ==================== WebinarLesson ====================
WebinarLesson::WebinarLesson(const std::string& title) {
    id_ = nextId_++;
    title_ = title;
    completed_ = false;
}
std::string WebinarLesson::getType() const { return "Вебинар"; }
double WebinarLesson::getWeight() const { return 1.5; }

// ==================== TextLesson ====================
TextLesson::TextLesson(const std::string& title) {
    id_ = nextId_++;
    title_ = title;
    completed_ = false;
}
std::string TextLesson::getType() const { return "Текстовый материал"; }
double TextLesson::getWeight() const { return 0.8; }

// ==================== InteractiveLesson ====================
InteractiveLesson::InteractiveLesson(const std::string& title, const std::string& question, const std::string& correctAnswer) {
    id_ = nextId_++;
    title_ = title;
    completed_ = false;
    question_ = question;
    correctAnswer_ = correctAnswer;
}
std::string InteractiveLesson::getType() const { return "Интерактивное задание"; }
double InteractiveLesson::getWeight() const { return 2.0; }
bool InteractiveLesson::checkAnswer(const std::string& answer) const { 
    return answer == correctAnswer_; 
}
std::string InteractiveLesson::getQuestion() const { 
    return question_; 
}

// ==================== Quiz ====================
Quiz::Quiz(const std::string& title) : title_(title) {}

void Quiz::addQuestion(const std::string& question, const std::string& correctAnswer) {
    questions_.push_back({question, correctAnswer});
}

bool Quiz::take() {
    std::cout << "\n--- Тест: " << title_ << " ---\n";
    int correct = 0;
    
    for (size_t i = 0; i < questions_.size(); ++i) {
        std::cout << "Вопрос " << (i + 1) << ": " << questions_[i].first << "\n";
        std::cout << "Ваш ответ: ";
        std::string answer;
        std::getline(std::cin >> std::ws, answer);
        
        if (answer == questions_[i].second) {
            std::cout << "  Правильно!\n";
            correct++;
        } else {
            std::cout << "  Неправильно. Правильный ответ: " << questions_[i].second << "\n";
        }
    }
    
    double percent = static_cast<double>(correct) / static_cast<double>(questions_.size()) * 100.0;
    std::cout << "\nРезультат: " << correct << "/" << questions_.size() << " (" << percent << "%)\n";
    
    if (percent >= 60) {
        std::cout << "Тест пройден!\n";
        return true;
    } else {
        std::cout << "Тест не пройден. Нужно 60% или больше.\n";
        return false;
    }
}

// ==================== Homework ====================
Homework::Homework(const std::string& description) 
    : description_(description), reviewed_(false), grade_(0) {}

void Homework::submit(const std::string& answer) {
    studentAnswer_ = answer;
    std::cout << "[OK] Домашнее задание отправлено на проверку.\n";
}

void Homework::review(int grade, const std::string& feedback) {
    reviewed_ = true;
    grade_ = grade;
    feedback_ = feedback;
    std::cout << "[OK] Домашнее задание проверено. Оценка: " << grade_ << "/100\n";
    std::cout << "Отзыв преподавателя: " << feedback_ << "\n";
}

// ==================== Module ====================
Module::Module(const std::string& name) : name_(name) {}

void Module::addLesson(std::unique_ptr<Lesson> lesson) {
    lessons_.push_back(std::move(lesson));
}

void Module::addQuiz(std::unique_ptr<Quiz> quiz) {
    quizzes_.push_back(std::move(quiz));
}

void Module::addHomework(std::unique_ptr<Homework> homework) {
    homeworks_.push_back(std::move(homework));
}

double Module::getProgress() const {
    double totalWeight = 0;
    double completedWeight = 0;
    
    for (const auto& l : lessons_) {
        totalWeight += l->getWeight();
        if (l->isCompleted()) completedWeight += l->getWeight();
    }
    
    for (const auto& h : homeworks_) {
        totalWeight += 2.0;
        if (h->isReviewed() && h->getGrade() >= 60) completedWeight += 2.0;
    }
    
    if (totalWeight < 0.001) return 0.0;
    return (completedWeight / totalWeight) * 100;
}

// ==================== Course ====================
Course::Course(const std::string& name) : name_(name), started_(0), completed_(0) {}

void Course::addModule(std::unique_ptr<Module> module) {
    modules_.push_back(std::move(module));
}

double Course::getOverallProgress() const {
    if (modules_.empty()) return 0;
    double sum = 0;
    for (const auto& m : modules_) {
        sum += m->getProgress();
    }
    return sum / static_cast<double>(modules_.size());
}

void Course::showStats() const {
    std::cout << "\n=== СТАТИСТИКА КУРСА \"" << name_ << "\" ===\n";
    std::cout << "Начали обучение: " << started_ << " чел.\n";
    std::cout << "Завершили: " << completed_ << " чел.\n";
    std::cout << "Текущий прогресс: " << getOverallProgress() << "%\n";
}

void Course::addReview(const std::string& review, int rating) {
    reviews_.push_back({review, rating});
    std::cout << "[OK] Отзыв добавлен.\n";
}

void Course::showReviews() const {
    if (reviews_.empty()) {
        std::cout << "Отзывов пока нет.\n";
        return;
    }
    std::cout << "\n=== ОТЗЫВЫ О КУРСЕ ===\n";
    double sumRating = 0;
    for (size_t i = 0; i < reviews_.size(); ++i) {
        std::cout << "  " << (i + 1) << ". Оценка: " << reviews_[i].second << "/5 - \"" << reviews_[i].first << "\"\n";
        sumRating += reviews_[i].second;
    }
    std::cout << "Средняя оценка: " << (sumRating / static_cast<double>(reviews_.size())) << "/5\n";
}

// ==================== OnlineCoursesSystem ====================
OnlineCoursesSystem::OnlineCoursesSystem() {
    studentName_ = "";
}

void OnlineCoursesSystem::createDemoCourse() {
    course_ = std::make_unique<Course>("Программирование на C++");
    
    // Модуль 1: Основы
    auto mod1 = std::make_unique<Module>("Модуль 1. Введение в C++");
    mod1->addLesson(std::make_unique<VideoLesson>("Установка и настройка среды"));
    mod1->addLesson(std::make_unique<TextLesson>("Синтаксис и типы данных"));
    mod1->addLesson(std::make_unique<InteractiveLesson>("Первая программа", 
        "Какой оператор выводит текст на экран?", "cout"));
    
    auto quiz1 = std::make_unique<Quiz>("Тест по основам C++");
    quiz1->addQuestion("Сколько байт в int?", "4");
    quiz1->addQuestion("Какой тип данных для символов?", "char");
    mod1->addQuiz(std::move(quiz1));
    
    mod1->addHomework(std::make_unique<Homework>("Написать программу, которая выводит 'Hello, World!'"));
    
    // Модуль 2: ООП
    auto mod2 = std::make_unique<Module>("Модуль 2. ООП в C++");
    mod2->addLesson(std::make_unique<WebinarLesson>("Классы и объекты"));
    mod2->addLesson(std::make_unique<VideoLesson>("Наследование и полиморфизм"));
    mod2->addLesson(std::make_unique<InteractiveLesson>("Конструкторы и деструкторы",
        "Как называется метод, вызываемый при создании объекта?", "конструктор"));
    
    auto quiz2 = std::make_unique<Quiz>("Тест по ООП");
    quiz2->addQuestion("Что такое инкапсуляция?", "сокрытие данных");
    quiz2->addQuestion("Какой принцип позволяет переопределять методы?", "полиморфизм");
    mod2->addQuiz(std::move(quiz2));
    
    mod2->addHomework(std::make_unique<Homework>("Создать класс Car с полями и методами"));
    
    course_->addModule(std::move(mod1));
    course_->addModule(std::move(mod2));
}

void OnlineCoursesSystem::showProgress() const {
    std::cout << "\n=== ПРОГРЕСС ОБУЧЕНИЯ ===\n";
    std::cout << "Студент: " << studentName_ << "\n";
    std::cout << "Курс: " << course_->getName() << "\n";
    std::cout << "Общий прогресс: " << course_->getOverallProgress() << "%\n";
    
    for (const auto& mod : course_->getModules()) {
        std::cout << "\n  " << mod->getName() << ": " << mod->getProgress() << "%\n";
        
        for (const auto& lesson : mod->getLessons()) {
            std::cout << "    [" << (lesson->isCompleted() ? "X" : " ") << "] " 
                      << lesson->getTitle() << " (" << lesson->getType() << ")\n";
        }
        
        for (const auto& hw : mod->getHomeworks()) {
            std::cout << "    [" << (hw->isReviewed() && hw->getGrade() >= 60 ? "X" : " ") << "] ДЗ: " 
                      << hw->getDescription();
            if (hw->isReviewed()) {
                std::cout << " (Оценка: " << hw->getGrade() << "/100)";
            }
            std::cout << "\n";
        }
    }
}

void OnlineCoursesSystem::showModules() const {
    std::cout << "\n=== МОДУЛИ КУРСА \"" << course_->getName() << "\" ===\n";
    for (size_t i = 0; i < course_->getModules().size(); ++i) {
        const auto& mod = course_->getModules()[i];
        std::cout << (i + 1) << ". " << mod->getName() << " (прогресс: " << mod->getProgress() << "%)\n";
    }
}

void OnlineCoursesSystem::studyModule() {
    showModules();
    std::cout << "Выберите модуль для изучения: ";
    int modChoice;
    std::cin >> modChoice;
    
    if (modChoice < 1 || modChoice > static_cast<int>(course_->getModules().size())) {
        std::cout << "Неверный выбор.\n";
        return;
    }
    
    auto& mod = course_->getModules()[static_cast<size_t>(modChoice - 1)];
    std::cout << "\n=== " << mod->getName() << " ===\n";
    
    for (auto& lesson : mod->getLessons()) {
        if (lesson->isCompleted()) {
            std::cout << "  [ПРОЙДЕНО] " << lesson->getTitle() << "\n";
            continue;
        }
        
        std::cout << "\n  Урок: " << lesson->getTitle() << " (" << lesson->getType() << ")\n";
        
        if (lesson->getType() == "Интерактивное задание") {
            std::cout << "  Вопрос: " << lesson->getQuestion() << "\n";
            std::cout << "  Ваш ответ: ";
            std::string answer;
            std::getline(std::cin >> std::ws, answer);
            
            if (lesson->checkAnswer(answer)) {
                std::cout << "  Правильно!\n";
                lesson->complete();
            } else {
                std::cout << "  Неправильно. Попробуйте еще раз позже.\n";
            }
        } else if (lesson->getType() == "Видео-лекция") {
            std::cout << "  [ИМИТАЦИЯ] Просмотр видео...\n";
            std::cout << "  Нажмите Enter когда просмотрите...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            lesson->complete();
        } else if (lesson->getType() == "Вебинар") {
            std::cout << "  [ИМИТАЦИЯ] Подключение к вебинару...\n";
            std::cout << "  Нажмите Enter когда прослушаете...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            lesson->complete();
        } else if (lesson->getType() == "Текстовый материал") {
            std::cout << "  [ИМИТАЦИЯ] Чтение материала...\n";
            std::cout << "  Нажмите Enter когда прочитаете...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            lesson->complete();
        }
    }
    
    std::cout << "\nПрогресс модуля: " << mod->getProgress() << "%\n";
}

void OnlineCoursesSystem::takeQuizzes() {
    showModules();
    std::cout << "Выберите модуль с тестом: ";
    int modChoice;
    std::cin >> modChoice;
    
    if (modChoice < 1 || modChoice > static_cast<int>(course_->getModules().size())) {
        std::cout << "Неверный выбор.\n";
        return;
    }
    
    auto& mod = course_->getModules()[static_cast<size_t>(modChoice - 1)];
    if (mod->getQuizzes().empty()) {
        std::cout << "В этом модуле нет тестов.\n";
        return;
    }
    
    for (auto& quiz : mod->getQuizzes()) {
        quiz->take();
    }
}

void OnlineCoursesSystem::submitHomework() {
    showModules();
    std::cout << "Выберите модуль с домашним заданием: ";
    int modChoice;
    std::cin >> modChoice;
    
    if (modChoice < 1 || modChoice > static_cast<int>(course_->getModules().size())) {
        std::cout << "Неверный выбор.\n";
        return;
    }
    
    auto& mod = course_->getModules()[static_cast<size_t>(modChoice - 1)];
    if (mod->getHomeworks().empty()) {
        std::cout << "В этом модуле нет домашних заданий.\n";
        return;
    }
    
    for (auto& hw : mod->getHomeworks()) {
        if (hw->isReviewed()) {
            std::cout << "[ПРОВЕРЕНО] " << hw->getDescription() << " - Оценка: " << hw->getGrade() << "/100\n";
            std::cout << "  Отзыв: " << hw->getFeedback() << "\n";
            continue;
        }
        
        std::cout << "\nДомашнее задание: " << hw->getDescription() << "\n";
        std::cout << "Введите ваш ответ (одной строкой): ";
        std::string answer;
        std::getline(std::cin >> std::ws, answer);
        hw->submit(answer);
        
        std::cout << "[ИМИТАЦИЯ] Преподаватель проверяет работу...\n";
        int grade = rand() % 31 + 70;
        hw->review(grade, "Хорошая работа, но можно улучшить оформление.");
    }
}

void OnlineCoursesSystem::leaveReview() {
    std::cout << "Ваша оценка курсу (1-5): ";
    int rating;
    std::cin >> rating;
    
    if (rating < 1 || rating > 5) {
        std::cout << "Оценка должна быть от 1 до 5.\n";
        return;
    }
    
    std::cout << "Ваш отзыв: ";
    std::string review;
    std::getline(std::cin >> std::ws, review);
    
    course_->addReview(review, rating);
}

void OnlineCoursesSystem::showStats() const {
    course_->showStats();
    course_->showReviews();
}

void OnlineCoursesSystem::generateCertificate() {
    double progress = course_->getOverallProgress();
    
    if (progress >= 100) {
        std::cout << "\nПоздравляем! Курс полностью пройден!\n";
        Certificate cert(studentName_, course_->getName());
        cert.print();
    } else {
        std::cout << "\nКурс пройден на " << progress << "%. Нужно 100% для получения сертификата.\n";
        std::cout << "Пройдите все уроки и сдайте домашние задания.\n";
    }
}

void OnlineCoursesSystem::run() {
    std::cout << "\n=== ОНЛАЙН-КУРСЫ ===\n";
    std::cout << "Введите ваше имя: ";
    std::getline(std::cin, studentName_);
    
    createDemoCourse();
    course_->incrementStarted();
    
    std::cout << "\nДобро пожаловать на курс \"" << course_->getName() << "\", " << studentName_ << "!\n";
    
    bool running = true;
    while (running) {
        std::cout << "\n=== МЕНЮ КУРСА ===\n";
        std::cout << "1. Посмотреть прогресс\n";
        std::cout << "2. Изучать модуль\n";
        std::cout << "3. Пройти тесты\n";
        std::cout << "4. Сдать домашнее задание\n";
        std::cout << "5. Оставить отзыв\n";
        std::cout << "6. Статистика курса\n";
        std::cout << "7. Получить сертификат\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";
        
        int choice;
        std::cin >> choice;
        
        try {
            switch (choice) {
                case 1: showProgress(); break;
                case 2: studyModule(); break;
                case 3: takeQuizzes(); break;
                case 4: submitHomework(); break;
                case 5: leaveReview(); break;
                case 6: showStats(); break;
                case 7: 
                    generateCertificate();
                    if (course_->getOverallProgress() >= 100 && course_->getCompleted() == 0) {
                        course_->incrementCompleted();
                    }
                    break;
                case 0: running = false; break;
                default: std::cout << "Неверный выбор.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
    }
}

void OnlineCoursesTask() {
    OnlineCoursesSystem system;
    system.run();
}