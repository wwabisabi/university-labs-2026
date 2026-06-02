#include "DatabaseManager.h"
#include "Exceptions.h"

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager(const std::string& host,
                                 const std::string& port,
                                 const std::string& dbname,
                                 const std::string& user,
                                 const std::string& password) {
    std::string connInfo =
        "host=" + host +
        " port=" + port +
        " dbname=" + dbname +
        " user=" + user +
        " password=" + password;

    conn = PQconnectdb(connInfo.c_str());

    if (PQstatus(conn) != CONNECTION_OK) {
        std::string errorMessage = PQerrorMessage(conn);
        PQfinish(conn);
        conn = nullptr;
        throw DatabaseException("Connection failed: " + errorMessage);
    }
}

DatabaseManager::~DatabaseManager() {
    if (conn != nullptr) {
        PQfinish(conn);
        conn = nullptr;
    }
}

DatabaseManager& DatabaseManager::getInstance(const std::string& host,
                                              const std::string& port,
                                              const std::string& dbname,
                                              const std::string& user,
                                              const std::string& password) {
    if (instance == nullptr) {
        instance = new DatabaseManager(host, port, dbname, user, password);
    }
    return *instance;
}

bool DatabaseManager::isConnected() const {
    return conn != nullptr && PQstatus(conn) == CONNECTION_OK;
}

PGresult* DatabaseManager::executeQuery(const std::string& sql) {
    PGresult* result = PQexec(conn, sql.c_str());

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::string errorMessage = PQerrorMessage(conn);
        PQclear(result);
        throw DatabaseException("SQL query failed: " + errorMessage);
    }

    return result;
}

void DatabaseManager::executeCommand(const std::string& sql) {
    PGresult* result = PQexec(conn, sql.c_str());

    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        std::string errorMessage = PQerrorMessage(conn);
        PQclear(result);
        throw DatabaseException("SQL command failed: " + errorMessage);
    }

    PQclear(result);
}

void DatabaseManager::beginTransaction() {
    executeCommand("BEGIN;");
}

void DatabaseManager::commitTransaction() {
    executeCommand("COMMIT;");
}

void DatabaseManager::rollbackTransaction() {
    executeCommand("ROLLBACK;");
}