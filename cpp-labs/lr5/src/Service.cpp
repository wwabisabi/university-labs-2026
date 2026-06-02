#include "Service.h"
#include <sstream>

Service::Service()
    : id(0), durationMinutes(0), price(0.0), materialCost(0.0) {}

Service::Service(int id,
                 const std::string& name,
                 int durationMinutes,
                 double price,
                 const std::string& requiredSpecialization,
                 double materialCost)
    : id(id),
      name(name),
      durationMinutes(durationMinutes),
      price(price),
      requiredSpecialization(requiredSpecialization),
      materialCost(materialCost) {}

int Service::getId() const {
    return id;
}

void Service::setId(int id) {
    this->id = id;
}

std::string Service::getName() const {
    return name;
}

void Service::setName(const std::string& name) {
    this->name = name;
}

int Service::getDurationMinutes() const {
    return durationMinutes;
}

void Service::setDurationMinutes(int durationMinutes) {
    this->durationMinutes = durationMinutes;
}

double Service::getPrice() const {
    return price;
}

void Service::setPrice(double price) {
    this->price = price;
}

std::string Service::getRequiredSpecialization() const {
    return requiredSpecialization;
}

void Service::setRequiredSpecialization(const std::string& requiredSpecialization) {
    this->requiredSpecialization = requiredSpecialization;
}

double Service::getMaterialCost() const {
    return materialCost;
}

void Service::setMaterialCost(double materialCost) {
    this->materialCost = materialCost;
}

std::string Service::toString() const {
    std::ostringstream oss;
    oss << "Service{id=" << id
        << ", name='" << name
        << "', durationMinutes=" << durationMinutes
        << ", price=" << price
        << ", requiredSpecialization='" << requiredSpecialization
        << "', materialCost=" << materialCost
        << "}";
    return oss.str();
}