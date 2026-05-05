#include <iostream>
#include <limits>
#include "Menu.h"
#include "Task1_Carsharing.h"
#include "Task2_OnlineCourses.h"
#include "Task3_Metro.h"
#include "Task4_VendingMachine.h"
#include "Task5_Airport.h"

int main() {
    setlocale(LC_ALL, "Russian");
    Menu menu;

    menu.addTask("Каршеринг", []() {
        CarSharingTask();
    });

    menu.addTask("Онлайн-курс", []() {
        OnlineCoursesTask();
    });

    menu.addTask("Московское метро", []() {
        MetroTask();
    });

    menu.addTask("Торговый автомат", []() {
        VendingMachineTask();
    });

    menu.addTask("Аэропорт", []() {
        AirportTask();
    });

    menu.run();
    return 0;
}