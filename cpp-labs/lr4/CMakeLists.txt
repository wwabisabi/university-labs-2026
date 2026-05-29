cmake_minimum_required(VERSION 3.10)
project(EvolutionSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Директория с заголовочными файлами
include_directories(include)

# Исходные файлы
set(SOURCES
    src/main.cpp
    src/creature.cpp
    src/herbivore.cpp
    src/predator.cpp
    src/omnivore.cpp
    src/tile.cpp
    src/map.cpp
    src/simulator.cpp
    src/statistics.cpp
)

# Создание исполняемого файла
add_executable(simulator ${SOURCES})

# Опции компилятора
if(MSVC)
    target_compile_options(simulator PRIVATE /W4)
else()
    target_compile_options(simulator PRIVATE -Wall -Wextra -pedantic)
endif()
