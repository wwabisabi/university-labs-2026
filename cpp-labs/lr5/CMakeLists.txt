cmake_minimum_required(VERSION 3.16)
project(beauty_salon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PostgreSQL REQUIRED)

add_executable(beauty_salon
    src/main.cpp

    src/Appointment.cpp
    src/Client.cpp
    src/DatabaseManager.cpp
    src/Employee.cpp
    src/EmployeeType.cpp
    src/SalaryLog.cpp
    src/SalonState.cpp
    src/Service.cpp

    src/AppointmentRepository.cpp
    src/ClientRepository.cpp
    src/EmployeeRepository.cpp
    src/SalaryLogRepository.cpp
    src/SalonStateRepository.cpp
    src/ServiceRepository.cpp

    src/BookingService.cpp
    src/FinanceService.cpp
    src/Menu.cpp
    src/SalaryService.cpp
    src/Utils.cpp
)

target_include_directories(beauty_salon PRIVATE
    include
    ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(beauty_salon PRIVATE PostgreSQL::PostgreSQL)