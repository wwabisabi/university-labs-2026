#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

// ==================== RAII: Сертификат ====================
class Certificate {
public:
    Certificate(const std::string& student, const std::string& course);
    ~Certificate();
    void print() const;
private:
    std::string student_;
    std::string course_;
    bool issued_;
};

// ==================== Уроки (полиморфизм) ====================
class Lesson {
public:
    virtual ~Lesson() = default;
    virtual std::string getType() const = 0;
    virtual double getWeight() const = 0;
    
    bool isCompleted() const { return completed_; }
    void complete();
    std::string getTitle() const { return title_; }
    int getId() const { return id_; }
    
    virtual bool checkAnswer(const std::string& /*answer*/) const { return true; }
    virtual std::string getQuestion() const { return ""; }
    
protected:
    int id_;
    std::string title_;
    bool completed_;
    static int nextId_;
};

class VideoLesson : public Lesson {
public:
    VideoLesson(const std::string& title);
    std::string getType() const override;
    double getWeight() const override;
};

class WebinarLesson : public Lesson {
public:
    WebinarLesson(const std::string& title);
    std::string getType() const override;
    double getWeight() const override;
};

class TextLesson : public Lesson {
public:
    TextLesson(const std::string& title);
    std::string getType() const override;
    double getWeight() const override;
};

class InteractiveLesson : public Lesson {
public:
    InteractiveLesson(const std::string& title, const std::string& question, const std::string& correctAnswer);
    std::string getType() const override;
    double getWeight() const override;
    bool checkAnswer(const std::string& answer) const override;
    std::string getQuestion() const override;
private:
    std::string question_;
    std::string correctAnswer_;
};

// ==================== Тест ====================
class Quiz {
public:
    Quiz(const std::string& title);
    void addQuestion(const std::string& question, const std::string& correctAnswer);
    bool take();
    std::string getTitle() const { return title_; }
private:
    std::string title_;
    std::vector<std::pair<std::string, std::string>> questions_;
};

// ==================== Домашнее задание ====================
class Homework {
public:
    Homework(const std::string& description);
    void submit(const std::string& answer);
    void review(int grade, const std::string& feedback);
    bool isReviewed() const { return reviewed_; }
    int getGrade() const { return grade_; }
    std::string getDescription() const { return description_; }
    std::string getFeedback() const { return feedback_; }
private:
    std::string description_;
    std::string studentAnswer_;
    bool reviewed_;
    int grade_;
    std::string feedback_;
};

// ==================== Модуль ====================
class Module {
public:
    Module(const std::string& name);
    void addLesson(std::unique_ptr<Lesson> lesson);
    void addQuiz(std::unique_ptr<Quiz> quiz);
    void addHomework(std::unique_ptr<Homework> homework);
    
    double getProgress() const;
    std::string getName() const { return name_; }
    std::vector<std::unique_ptr<Lesson>>& getLessons() { return lessons_; }
    std::vector<std::unique_ptr<Quiz>>& getQuizzes() { return quizzes_; }
    std::vector<std::unique_ptr<Homework>>& getHomeworks() { return homeworks_; }
    
private:
    std::string name_;
    std::vector<std::unique_ptr<Lesson>> lessons_;
    std::vector<std::unique_ptr<Quiz>> quizzes_;
    std::vector<std::unique_ptr<Homework>> homeworks_;
};

// ==================== Курс ====================
class Course {
public:
    Course(const std::string& name);
    void addModule(std::unique_ptr<Module> module);
    
    double getOverallProgress() const;
    std::string getName() const { return name_; }
    std::vector<std::unique_ptr<Module>>& getModules() { return modules_; }
    
    void showStats() const;
    void addReview(const std::string& review, int rating);
    void showReviews() const;
    
    void incrementStarted() { started_++; }
    void incrementCompleted() { completed_++; }
    int getStarted() const { return started_; }
    int getCompleted() const { return completed_; }
    
private:
    std::string name_;
    std::vector<std::unique_ptr<Module>> modules_;
    std::vector<std::pair<std::string, int>> reviews_;
    int started_;
    int completed_;
};

// ==================== Система онлайн-курсов ====================
class OnlineCoursesSystem {
public:
    OnlineCoursesSystem();
    void run();
    
private:
    std::unique_ptr<Course> course_;
    std::string studentName_;
    
    void createDemoCourse();
    void showProgress() const;
    void showModules() const;
    void studyModule();
    void takeQuizzes();
    void submitHomework();
    void leaveReview();
    void showStats() const;
    void generateCertificate();
};

void OnlineCoursesTask();