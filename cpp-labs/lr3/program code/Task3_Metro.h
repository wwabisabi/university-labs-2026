#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>

class Station {
public:
    Station(const std::string& name);
    
    void addLine(const std::string& lineName, const std::string& color);
    void addNeighbor(Station* station, int time);
    void addTransfer(Station* station, int time);
    
    const std::string& getName() const { return name_; }
    const std::vector<std::pair<std::string, std::string>>& getLines() const { return lines_; }
    const std::vector<std::pair<Station*, int>>& getNeighbors() const { return neighbors_; }
    const std::vector<std::pair<Station*, int>>& getTransfers() const { return transfers_; }
    
private:
    std::string name_;
    std::vector<std::pair<std::string, std::string>> lines_;
    std::vector<std::pair<Station*, int>> neighbors_;
    std::vector<std::pair<Station*, int>> transfers_;
};

class MetroSystem {
public:
    MetroSystem();
    void run();
    
private:
    std::vector<std::unique_ptr<Station>> stations_;
    std::vector<Station*> closedStations_;
    
    void createMetroMap();
    Station* findStation(const std::string& name) const;
    
    void showAllStations() const;
    void showStationInfo() const;
    void showSchedule() const;
    void findShortestPath();
    void simulateEmergency();
    void removeEmergency();
    void showClosedStations() const;
    
    std::vector<Station*> dijkstra(Station* start, Station* end) const;
};

void MetroTask();