#include "Task3_Metro.h"
#include <iostream>
#include <limits>

// ==================== Station ====================
Station::Station(const std::string& name) : name_(name) {}

void Station::addLine(const std::string& lineName, const std::string& color) {
    lines_.push_back({lineName, color});
}

void Station::addNeighbor(Station* station, int time) {
    neighbors_.push_back({station, time});
}

void Station::addTransfer(Station* station, int time) {
    transfers_.push_back({station, time});
}

// ==================== MetroSystem ====================
MetroSystem::MetroSystem() {
    createMetroMap();
}

void MetroSystem::createMetroMap() {
    // ==================== СОКОЛЬНИЧЕСКАЯ ЛИНИЯ (красная) ====================
    auto komsomolskaya = std::make_unique<Station>("Комсомольская");
    komsomolskaya->addLine("Сокольническая", "Красный");
    komsomolskaya->addLine("Кольцевая", "Коричневый");
    
    auto krasnye_vorota = std::make_unique<Station>("Красные ворота");
    krasnye_vorota->addLine("Сокольническая", "Красный");
    
    auto chistye_prudy = std::make_unique<Station>("Чистые пруды");
    chistye_prudy->addLine("Сокольническая", "Красный");
    
    auto lubyanka = std::make_unique<Station>("Лубянка");
    lubyanka->addLine("Сокольническая", "Красный");
    
    auto ohotny_ryad = std::make_unique<Station>("Охотный ряд");
    ohotny_ryad->addLine("Сокольническая", "Красный");
    
    auto biblioteka_lenina = std::make_unique<Station>("Библиотека им. Ленина");
    biblioteka_lenina->addLine("Сокольническая", "Красный");
    
    auto kropotkinskaya = std::make_unique<Station>("Кропоткинская");
    kropotkinskaya->addLine("Сокольническая", "Красный");
    
    auto park_kultury = std::make_unique<Station>("Парк культуры");
    park_kultury->addLine("Сокольническая", "Красный");
    park_kultury->addLine("Кольцевая", "Коричневый");
    
    // ==================== АРБАТСКО-ПОКРОВСКАЯ ЛИНИЯ (синяя) ====================
    auto kievskaya = std::make_unique<Station>("Киевская");
    kievskaya->addLine("Арбатско-Покровская", "Синий");
    kievskaya->addLine("Кольцевая", "Коричневый");
    
    auto smolenskaya = std::make_unique<Station>("Смоленская");
    smolenskaya->addLine("Арбатско-Покровская", "Синий");
    
    auto arbatskaya = std::make_unique<Station>("Арбатская");
    arbatskaya->addLine("Арбатско-Покровская", "Синий");
    
    auto ploshad_revolutsii = std::make_unique<Station>("Площадь Революции");
    ploshad_revolutsii->addLine("Арбатско-Покровская", "Синий");
    
    auto kurskaya = std::make_unique<Station>("Курская");
    kurskaya->addLine("Арбатско-Покровская", "Синий");
    kurskaya->addLine("Кольцевая", "Коричневый");
    
    // ==================== ЗАМОСКВОРЕЦКАЯ ЛИНИЯ (зелёная) ====================
    auto belorusskaya = std::make_unique<Station>("Белорусская");
    belorusskaya->addLine("Замоскворецкая", "Зелёный");
    belorusskaya->addLine("Кольцевая", "Коричневый");
    
    auto mayakovskaya = std::make_unique<Station>("Маяковская");
    mayakovskaya->addLine("Замоскворецкая", "Зелёный");
    
    auto tverskaya = std::make_unique<Station>("Тверская");
    tverskaya->addLine("Замоскворецкая", "Зелёный");
    
    auto teatralnaya = std::make_unique<Station>("Театральная");
    teatralnaya->addLine("Замоскворецкая", "Зелёный");
    
    auto novokuznetskaya = std::make_unique<Station>("Новокузнецкая");
    novokuznetskaya->addLine("Замоскворецкая", "Зелёный");
    
    auto paveltskaya = std::make_unique<Station>("Павелецкая");
    paveltskaya->addLine("Замоскворецкая", "Зелёный");
    paveltskaya->addLine("Кольцевая", "Коричневый");
    
    // ==================== СОХРАНЯЕМ УКАЗАТЕЛИ ====================
    Station* komsomolskaya_ptr = komsomolskaya.get();
    Station* krasnye_vorota_ptr = krasnye_vorota.get();
    Station* chistye_prudy_ptr = chistye_prudy.get();
    Station* lubyanka_ptr = lubyanka.get();
    Station* ohotny_ryad_ptr = ohotny_ryad.get();
    Station* biblioteka_lenina_ptr = biblioteka_lenina.get();
    Station* kropotkinskaya_ptr = kropotkinskaya.get();
    Station* park_kultury_ptr = park_kultury.get();
    Station* kievskaya_ptr = kievskaya.get();
    Station* smolenskaya_ptr = smolenskaya.get();
    Station* arbatskaya_ptr = arbatskaya.get();
    Station* ploshad_revolutsii_ptr = ploshad_revolutsii.get();
    Station* kurskaya_ptr = kurskaya.get();
    Station* belorusskaya_ptr = belorusskaya.get();
    Station* mayakovskaya_ptr = mayakovskaya.get();
    Station* tverskaya_ptr = tverskaya.get();
    Station* teatralnaya_ptr = teatralnaya.get();
    Station* novokuznetskaya_ptr = novokuznetskaya.get();
    Station* paveltskaya_ptr = paveltskaya.get();
    
    // ==================== СВЯЗИ: СОКОЛЬНИЧЕСКАЯ ЛИНИЯ ====================
    komsomolskaya_ptr->addNeighbor(krasnye_vorota_ptr, 2);
    krasnye_vorota_ptr->addNeighbor(komsomolskaya_ptr, 2);
    
    krasnye_vorota_ptr->addNeighbor(chistye_prudy_ptr, 1);
    chistye_prudy_ptr->addNeighbor(krasnye_vorota_ptr, 1);
    
    chistye_prudy_ptr->addNeighbor(lubyanka_ptr, 1);
    lubyanka_ptr->addNeighbor(chistye_prudy_ptr, 1);
    
    lubyanka_ptr->addNeighbor(ohotny_ryad_ptr, 1);
    ohotny_ryad_ptr->addNeighbor(lubyanka_ptr, 1);
    
    ohotny_ryad_ptr->addNeighbor(biblioteka_lenina_ptr, 1);
    biblioteka_lenina_ptr->addNeighbor(ohotny_ryad_ptr, 1);
    
    biblioteka_lenina_ptr->addNeighbor(kropotkinskaya_ptr, 2);
    kropotkinskaya_ptr->addNeighbor(biblioteka_lenina_ptr, 2);
    
    kropotkinskaya_ptr->addNeighbor(park_kultury_ptr, 1);
    park_kultury_ptr->addNeighbor(kropotkinskaya_ptr, 1);
    
    // ==================== СВЯЗИ: АРБАТСКО-ПОКРОВСКАЯ ЛИНИЯ ====================
    kievskaya_ptr->addNeighbor(smolenskaya_ptr, 2);
    smolenskaya_ptr->addNeighbor(kievskaya_ptr, 2);
    
    smolenskaya_ptr->addNeighbor(arbatskaya_ptr, 2);
    arbatskaya_ptr->addNeighbor(smolenskaya_ptr, 2);
    
    arbatskaya_ptr->addNeighbor(ploshad_revolutsii_ptr, 2);
    ploshad_revolutsii_ptr->addNeighbor(arbatskaya_ptr, 2);
    
    ploshad_revolutsii_ptr->addNeighbor(kurskaya_ptr, 2);
    kurskaya_ptr->addNeighbor(ploshad_revolutsii_ptr, 2);
    
    // ==================== СВЯЗИ: ЗАМОСКВОРЕЦКАЯ ЛИНИЯ ====================
    belorusskaya_ptr->addNeighbor(mayakovskaya_ptr, 2);
    mayakovskaya_ptr->addNeighbor(belorusskaya_ptr, 2);
    
    mayakovskaya_ptr->addNeighbor(tverskaya_ptr, 1);
    tverskaya_ptr->addNeighbor(mayakovskaya_ptr, 1);
    
    tverskaya_ptr->addNeighbor(teatralnaya_ptr, 1);
    teatralnaya_ptr->addNeighbor(tverskaya_ptr, 1);
    
    teatralnaya_ptr->addNeighbor(novokuznetskaya_ptr, 2);
    novokuznetskaya_ptr->addNeighbor(teatralnaya_ptr, 2);
    
    novokuznetskaya_ptr->addNeighbor(paveltskaya_ptr, 2);
    paveltskaya_ptr->addNeighbor(novokuznetskaya_ptr, 2);
    
    // ==================== ПЕРЕСАДКИ ====================
    
    // Комсомольская: красная <-> кольцевая
    komsomolskaya_ptr->addTransfer(komsomolskaya_ptr, 3);
    
    // Парк культуры: красная <-> кольцевая
    park_kultury_ptr->addTransfer(park_kultury_ptr, 3);
    
    // Киевская: синяя <-> кольцевая
    kievskaya_ptr->addTransfer(kievskaya_ptr, 3);
    
    // Курская: синяя <-> кольцевая
    kurskaya_ptr->addTransfer(kurskaya_ptr, 3);
    
    // Белорусская: зелёная <-> кольцевая
    belorusskaya_ptr->addTransfer(belorusskaya_ptr, 3);
    
    // Павелецкая: зелёная <-> кольцевая
    paveltskaya_ptr->addTransfer(paveltskaya_ptr, 3);
    
    // Охотный ряд (красная) <-> Театральная (зелёная)
    ohotny_ryad_ptr->addTransfer(teatralnaya_ptr, 2);
    teatralnaya_ptr->addTransfer(ohotny_ryad_ptr, 2);
    
    // Театральная (зелёная) <-> Площадь Революции (синяя)
    teatralnaya_ptr->addTransfer(ploshad_revolutsii_ptr, 2);
    ploshad_revolutsii_ptr->addTransfer(teatralnaya_ptr, 2);
    
    // Охотный ряд (красная) <-> Площадь Революции (синяя) — через общий узел
    ohotny_ryad_ptr->addTransfer(ploshad_revolutsii_ptr, 3);
    ploshad_revolutsii_ptr->addTransfer(ohotny_ryad_ptr, 3);
    
    // Библиотека им. Ленина (красная) <-> Арбатская (синяя)
    biblioteka_lenina_ptr->addTransfer(arbatskaya_ptr, 2);
    arbatskaya_ptr->addTransfer(biblioteka_lenina_ptr, 2);
    
    // ==================== ДОБАВЛЯЕМ СТАНЦИИ ====================
    stations_.push_back(std::move(komsomolskaya));
    stations_.push_back(std::move(krasnye_vorota));
    stations_.push_back(std::move(chistye_prudy));
    stations_.push_back(std::move(lubyanka));
    stations_.push_back(std::move(ohotny_ryad));
    stations_.push_back(std::move(biblioteka_lenina));
    stations_.push_back(std::move(kropotkinskaya));
    stations_.push_back(std::move(park_kultury));
    stations_.push_back(std::move(kievskaya));
    stations_.push_back(std::move(smolenskaya));
    stations_.push_back(std::move(arbatskaya));
    stations_.push_back(std::move(ploshad_revolutsii));
    stations_.push_back(std::move(kurskaya));
    stations_.push_back(std::move(belorusskaya));
    stations_.push_back(std::move(mayakovskaya));
    stations_.push_back(std::move(tverskaya));
    stations_.push_back(std::move(teatralnaya));
    stations_.push_back(std::move(novokuznetskaya));
    stations_.push_back(std::move(paveltskaya));
}

Station* MetroSystem::findStation(const std::string& name) const {
    auto it = std::find_if(stations_.begin(), stations_.end(),
        [&name](const auto& s) { return s->getName() == name; });
    if (it != stations_.end()) return it->get();
    return nullptr;
}

void MetroSystem::showAllStations() const {
    std::cout << "\n=== СТАНЦИИ МЕТРО ===\n";
    
    std::cout << "\nСокольническая линия (красная):\n";
    std::cout << "  Комсомольская, Красные ворота, Чистые пруды, Лубянка, Охотный ряд, Библиотека им. Ленина, Кропоткинская, Парк культуры\n";
    
    std::cout << "\nАрбатско-Покровская линия (синяя):\n";
    std::cout << "  Киевская, Смоленская, Арбатская, Площадь Революции, Курская\n";
    
    std::cout << "\nЗамоскворецкая линия (зелёная):\n";
    std::cout << "  Белорусская, Маяковская, Тверская, Театральная, Новокузнецкая, Павелецкая\n";
    
    std::cout << "\nПересадочные узлы:\n";
    std::cout << "  Комсомольская: Сокольническая <-> Кольцевая\n";
    std::cout << "  Парк культуры: Сокольническая <-> Кольцевая\n";
    std::cout << "  Киевская: Арбатско-Покровская <-> Кольцевая\n";
    std::cout << "  Курская: Арбатско-Покровская <-> Кольцевая\n";
    std::cout << "  Белорусская: Замоскворецкая <-> Кольцевая\n";
    std::cout << "  Павелецкая: Замоскворецкая <-> Кольцевая\n";
    std::cout << "  Охотный ряд <-> Театральная <-> Площадь Революции\n";
    std::cout << "  Библиотека им. Ленина <-> Арбатская\n";
    
    if (!closedStations_.empty()) {
        std::cout << "\nЗакрытые станции:\n";
        for (const auto& s : closedStations_) {
            std::cout << "  - " << s->getName() << "\n";
        }
    }
}

void MetroSystem::showStationInfo() const {
    std::cout << "Введите название станции: ";
    std::string name;
    std::getline(std::cin >> std::ws, name);
    
    Station* station = findStation(name);
    if (!station) {
        std::cout << "Станция не найдена.\n";
        return;
    }
    
    std::cout << "\nСтанция: " << station->getName() << "\n";
    
    std::cout << "Линии: ";
    for (size_t i = 0; i < station->getLines().size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << station->getLines()[i].first << " (" << station->getLines()[i].second << ")";
    }
    std::cout << "\n";
    
    if (!station->getNeighbors().empty()) {
        std::cout << "Соседние станции:\n";
        for (const auto& neighbor : station->getNeighbors()) {
            std::cout << "  - " << neighbor.first->getName() << " (" << neighbor.second << " мин)\n";
        }
    }
    
    if (!station->getTransfers().empty()) {
        std::cout << "Пересадки:\n";
        for (const auto& transfer : station->getTransfers()) {
            std::cout << "  - На " << transfer.first->getName() << " (" << transfer.second << " мин)\n";
        }
    }
    
    bool closed = std::find(closedStations_.begin(), closedStations_.end(), station) != closedStations_.end();
    std::cout << "Статус: " << (closed ? "ЗАКРЫТА" : "Открыта") << "\n";
}

void MetroSystem::showSchedule() const {
    std::cout << "\nРасписание движения поездов:\n";
    std::cout << "  Часы пик (7:00-10:00, 17:00-20:00): интервал 2 мин\n";
    std::cout << "  Дневное время: интервал 5 мин\n";
    std::cout << "  Ночное время (23:00-5:00): интервал 10 мин\n";
}

void MetroSystem::showClosedStations() const {
    if (closedStations_.empty()) {
        std::cout << "Все станции открыты.\n";
        return;
    }
    std::cout << "\nЗакрытые станции:\n";
    for (const auto& station : closedStations_) {
        std::cout << "  - " << station->getName() << "\n";
    }
}

std::vector<Station*> MetroSystem::dijkstra(Station* start, Station* end) const {
    std::unordered_map<Station*, int> dist;
    std::unordered_map<Station*, Station*> prev;
    
    for (const auto& s : stations_) {
        dist[s.get()] = std::numeric_limits<int>::max();
    }
    dist[start] = 0;
    
    auto cmp = [](const std::pair<int, Station*>& a, const std::pair<int, Station*>& b) { 
        return a.first > b.first; 
    };
    std::priority_queue<std::pair<int, Station*>, std::vector<std::pair<int, Station*>>, decltype(cmp)> pq(cmp);
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); 
        pq.pop();
        
        if (d > dist[u]) continue;
        if (u == end) break;
        
        for (auto& [v, w] : u->getNeighbors()) {
            if (std::find(closedStations_.begin(), closedStations_.end(), v) != closedStations_.end())
                continue;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
        
        for (auto& [v, w] : u->getTransfers()) {
            if (std::find(closedStations_.begin(), closedStations_.end(), v) != closedStations_.end())
                continue;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    
    if (dist[end] == std::numeric_limits<int>::max()) {
        return {};
    }
    
    std::vector<Station*> path;
    Station* current = end;
    while (current != nullptr) {
        path.push_back(current);
        if (current == start) break;
        current = prev[current];
    }
    std::reverse(path.begin(), path.end());
    
    return path;
}

void MetroSystem::findShortestPath() {
    showAllStations();
    
    std::cout << "\nВведите начальную станцию: ";
    std::string from;
    std::getline(std::cin >> std::ws, from);
    
    std::cout << "Введите конечную станцию: ";
    std::string to;
    std::getline(std::cin >> std::ws, to);
    
    Station* start = findStation(from);
    Station* end = findStation(to);
    
    if (!start) {
        std::cout << "Начальная станция не найдена.\n";
        return;
    }
    if (!end) {
        std::cout << "Конечная станция не найдена.\n";
        return;
    }
    
    if (std::find(closedStations_.begin(), closedStations_.end(), start) != closedStations_.end()) {
        std::cout << "Начальная станция закрыта.\n";
        return;
    }
    if (std::find(closedStations_.begin(), closedStations_.end(), end) != closedStations_.end()) {
        std::cout << "Конечная станция закрыта.\n";
        return;
    }
    
    std::vector<Station*> path = dijkstra(start, end);
    
    if (path.empty()) {
        std::cout << "Маршрут не найден. Возможно, все пути заблокированы.\n";
        return;
    }
    
    int totalTime = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        bool found = false;
        for (const auto& neighbor : path[i]->getNeighbors()) {
            if (neighbor.first == path[i + 1]) {
                totalTime += neighbor.second;
                found = true;
                break;
            }
        }
        if (!found) {
            for (const auto& transfer : path[i]->getTransfers()) {
                if (transfer.first == path[i + 1]) {
                    totalTime += transfer.second;
                    found = true;
                    break;
                }
            }
        }
    }
    
    std::cout << "\nМаршрут: " << from << " -> " << to << "\n";
    std::cout << "Общее время: " << totalTime << " мин.\n\n";
    
    std::string currentLine = "";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << path[i]->getName();
        
        if (!path[i]->getLines().empty()) {
            std::cout << " (" << path[i]->getLines()[0].first << ")";
        }
        
        if (i < path.size() - 1) {
            bool isTransfer = true;
            for (const auto& neighbor : path[i]->getNeighbors()) {
                if (neighbor.first == path[i + 1]) {
                    std::cout << " -> " << neighbor.second << " мин";
                    isTransfer = false;
                    break;
                }
            }
            if (isTransfer) {
                for (const auto& transfer : path[i]->getTransfers()) {
                    if (transfer.first == path[i + 1]) {
                        std::cout << " [пересадка " << transfer.second << " мин]";
                        break;
                    }
                }
            }
        }
        
        std::cout << "\n";
    }
}

void MetroSystem::simulateEmergency() {
    showAllStations();
    
    std::cout << "\nВведите название станции для закрытия: ";
    std::string name;
    std::getline(std::cin >> std::ws, name);
    
    Station* station = findStation(name);
    if (!station) {
        std::cout << "Станция не найдена.\n";
        return;
    }
    
    if (std::find(closedStations_.begin(), closedStations_.end(), station) != closedStations_.end()) {
        std::cout << "Станция уже закрыта.\n";
        return;
    }
    
    closedStations_.push_back(station);
    std::cout << "Авария! Станция \"" << name << "\" закрыта. Поезда проезжают без остановки.\n";
}

void MetroSystem::removeEmergency() {
    if (closedStations_.empty()) {
        std::cout << "Нет закрытых станций.\n";
        return;
    }
    
    showClosedStations();
    
    std::cout << "\nВведите название станции для открытия: ";
    std::string name;
    std::getline(std::cin >> std::ws, name);
    
    auto it = std::find_if(closedStations_.begin(), closedStations_.end(),
        [&name](const Station* s) { return s->getName() == name; });
    
    if (it != closedStations_.end()) {
        closedStations_.erase(it);
        std::cout << "Станция \"" << name << "\" открыта. Движение восстановлено.\n";
    } else {
        std::cout << "Станция не найдена среди закрытых.\n";
    }
}

void MetroSystem::run() {
    std::cout << "\n=== МОСКОВСКОЕ МЕТРО ===\n";
    std::cout << "Симулятор метрополитена (станции в пределах Кольцевой линии)\n";
    
    bool running = true;
    while (running) {
        std::cout << "\n1. Показать все станции\n";
        std::cout << "2. Информация о станции\n";
        std::cout << "3. Расписание движения\n";
        std::cout << "4. Построить маршрут\n";
        std::cout << "5. Авария (закрыть станцию)\n";
        std::cout << "6. Восстановить станцию\n";
        std::cout << "7. Закрытые станции\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";
        
        int choice;
        std::cin >> choice;
        
        try {
            switch (choice) {
                case 1: showAllStations(); break;
                case 2: showStationInfo(); break;
                case 3: showSchedule(); break;
                case 4: findShortestPath(); break;
                case 5: simulateEmergency(); break;
                case 6: removeEmergency(); break;
                case 7: showClosedStations(); break;
                case 0: running = false; break;
                default: std::cout << "Неверный выбор.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
    }
}

void MetroTask() {
    MetroSystem metro;
    metro.run();
}