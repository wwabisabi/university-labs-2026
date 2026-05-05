#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <ctime>
#include <cstdlib>

class Passenger {
public:
    virtual ~Passenger() = default;
    virtual std::string getType() const = 0;
    virtual int getPriority() const = 0;
    virtual double getBaggageAllowance() const = 0;
    
    std::string getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
protected:
    std::string name_;
};

class EconomyPassenger : public Passenger {
public:
    EconomyPassenger(const std::string& name);
    std::string getType() const override;
    int getPriority() const override;
    double getBaggageAllowance() const override;
};

class BusinessPassenger : public Passenger {
public:
    BusinessPassenger(const std::string& name);
    std::string getType() const override;
    int getPriority() const override;
    double getBaggageAllowance() const override;
};

class VipPassenger : public Passenger {
public:
    VipPassenger(const std::string& name);
    std::string getType() const override;
    int getPriority() const override;
    double getBaggageAllowance() const override;
};

class Ticket {
public:
    Ticket(std::shared_ptr<Passenger> passenger, const std::string& flightNumber, double baggageWeight);
    
    std::shared_ptr<Passenger> getPassenger() const { return passenger_; }
    std::string getFlightNumber() const { return flightNumber_; }
    double getBaggageWeight() const { return baggageWeight_; }
    double getBaggageFee() const { return baggageFee_; }
    bool isOnlineCheckIn() const { return onlineCheckIn_; }
    void setOnlineCheckIn(bool value) { onlineCheckIn_ = value; }
    
private:
    std::shared_ptr<Passenger> passenger_;
    std::string flightNumber_;
    double baggageWeight_;
    double baggageFee_;
    bool onlineCheckIn_;
};

class Flight {
public:
    Flight(const std::string& number, const std::string& destination, int capacity, time_t departureTime);
    
    std::string getNumber() const { return number_; }
    std::string getDestination() const { return destination_; }
    int getCapacity() const { return capacity_; }
    int getBooked() const { return static_cast<int>(tickets_.size()); }
    bool isFull() const { return static_cast<int>(tickets_.size()) >= capacity_; }
    bool isDelayed() const { return delayed_; }
    std::string getDelayReason() const { return delayReason_; }
    time_t getDepartureTime() const { return departureTime_; }
    
    bool addTicket(std::unique_ptr<Ticket> ticket);
    void board();
    void delay(const std::string& reason);
    void showLoad() const;
    double getLoadPercentage() const;
    const std::vector<std::unique_ptr<Ticket>>& getTickets() const { return tickets_; }
    
private:
    std::string number_;
    std::string destination_;
    int capacity_;
    time_t departureTime_;
    bool delayed_;
    std::string delayReason_;
    std::vector<std::unique_ptr<Ticket>> tickets_;
};

class Transfer {
public:
    Transfer(const std::string& fromFlight, const std::string& toFlight, int minTransferTime);
    
    std::string getFromFlight() const { return fromFlight_; }
    std::string getToFlight() const { return toFlight_; }
    int getMinTransferTime() const { return minTransferTime_; }
    bool canMakeTransfer(time_t arrivalTime, time_t departureTime) const;
    
private:
    std::string fromFlight_;
    std::string toFlight_;
    int minTransferTime_;
};

class Airport {
public:
    Airport();
    void run();
    
private:
    std::vector<std::unique_ptr<Flight>> flights_;
    std::vector<Transfer> transfers_;
    
    void setupDemo();
    Flight* findFlight(const std::string& number);
    Flight* findFlight(const std::string& number) const;
    
    void showFlights() const;
    void showFlightDetails() const;
    void registerPassenger();
    void registerOnline();
    void boardFlight();
    void delayFlight();
    void checkTransfer();
    void showStats() const;
};

void AirportTask();