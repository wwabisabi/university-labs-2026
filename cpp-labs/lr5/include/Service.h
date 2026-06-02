#ifndef SERVICE_H
#define SERVICE_H

#include <string>

class Service {
private:
    int id;
    std::string name;
    int durationMinutes;
    double price;
    std::string requiredSpecialization;
    double materialCost;

public:
    Service();
    Service(int id,
            const std::string& name,
            int durationMinutes,
            double price,
            const std::string& requiredSpecialization,
            double materialCost);

    int getId() const;
    void setId(int id);

    std::string getName() const;
    void setName(const std::string& name);

    int getDurationMinutes() const;
    void setDurationMinutes(int durationMinutes);

    double getPrice() const;
    void setPrice(double price);

    std::string getRequiredSpecialization() const;
    void setRequiredSpecialization(const std::string& requiredSpecialization);

    double getMaterialCost() const;
    void setMaterialCost(double materialCost);

    std::string toString() const;
};

#endif