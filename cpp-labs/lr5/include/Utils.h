#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace utils {
    int read_int(const std::string& prompt);
    double read_double(const std::string& prompt);
    std::string read_string(const std::string& prompt);
    void print_header(const std::string& title);
    void press_enter_to_continue();
    bool confirm(const std::string& prompt);
}

#endif