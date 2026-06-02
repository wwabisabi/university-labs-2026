#ifndef CLIENT_REPOSITORY_H
#define CLIENT_REPOSITORY_H

#include <vector>
#include "Client.h"
#include "DatabaseManager.h"

class ClientRepository {
private:
    DatabaseManager& db;

public:
    explicit ClientRepository(DatabaseManager& db);

    std::vector<Client> getAll();
    Client getById(int id);
    void add(const Client& client);
    void update(const Client& client);
    void softDelete(int id);
};

#endif