#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
private:
    int id;
    std::string fullName;
    std::string phone;
    std::string firstVisitDate;
    double totalSpent;
    int visitsCount;
    int bonusPoints;
    bool isDeleted;

public:
    Client();
    Client(int id,
           const std::string& fullName,
           const std::string& phone,
           const std::string& firstVisitDate,
           double totalSpent,
           int visitsCount,
           int bonusPoints,
           bool isDeleted);

    int getId() const;
    void setId(int id);

    std::string getFullName() const;
    void setFullName(const std::string& fullName);

    std::string getPhone() const;
    void setPhone(const std::string& phone);

    std::string getFirstVisitDate() const;
    void setFirstVisitDate(const std::string& firstVisitDate);

    double getTotalSpent() const;
    void setTotalSpent(double totalSpent);

    int getVisitsCount() const;
    void setVisitsCount(int visitsCount);

    int getBonusPoints() const;
    void setBonusPoints(int bonusPoints);

    bool getIsDeleted() const;
    void setIsDeleted(bool isDeleted);

    std::string toString() const;
};

#endif