#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <libpq-fe.h>

class DatabaseManager {
private:
    static DatabaseManager* instance;
    PGconn* conn;

    DatabaseManager(const std::string& host,
                    const std::string& port,
                    const std::string& dbname,
                    const std::string& user,
                    const std::string& password);

public:
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    static DatabaseManager& getInstance(const std::string& host = "localhost",
                                        const std::string& port = "5432",
                                        const std::string& dbname = "beauty_salon_db",
                                        const std::string& user = "postgres",
                                        const std::string& password = "postgres");

    bool isConnected() const;

    PGresult* executeQuery(const std::string& sql);
    void executeCommand(const std::string& sql);

    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();
};

#endif