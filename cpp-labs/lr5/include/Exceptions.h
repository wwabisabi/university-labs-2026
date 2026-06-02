#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class DatabaseException : public std::runtime_error {
public:
    explicit DatabaseException(const std::string& message)
        : std::runtime_error(message) {}
};

class NotFoundException : public std::runtime_error {
public:
    explicit NotFoundException(const std::string& message)
        : std::runtime_error(message) {}
};

class InvalidInputException : public std::runtime_error {
public:
    explicit InvalidInputException(const std::string& message)
        : std::runtime_error(message) {}
};

class NoFreeSlotException : public std::runtime_error {
public:
    explicit NoFreeSlotException(const std::string& message)
        : std::runtime_error(message) {}
};

class InsufficientFundsException : public std::runtime_error {
public:
    explicit InsufficientFundsException(const std::string& message)
        : std::runtime_error(message) {}
};

class ForbiddenActionException : public std::runtime_error {
public:
    explicit ForbiddenActionException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif