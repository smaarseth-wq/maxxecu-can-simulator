#include "canbus.h"

#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

static bool running = true;

void signalHandler(int)
{
    running = false;
}

int main()
{
    std::signal(SIGINT, signalHandler);

    std::cout << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << " MaxxECU CAN Simulator Release_001" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << std::endl;

    CanBus can;

    std::cout << "Opening CAN interface..." << std::endl;

    if (!can.open("can0"))
    {
        std::cout << std::endl;
        std::cout << "ERROR: Failed to open can0" << std::endl;
        return -1;
    }

    std::cout << "CAN OK" << std::endl;
    std::cout << std::endl;

    uint16_t rpm = 800;
    int direction = 1;

    while (running)
    {
        can.sendRPM(rpm);

        rpm += (direction * 100);

        if (rpm >= 8000)
        {
            rpm = 8000;
            direction = -1;
        }

        if (rpm <= 800)
        {
            rpm = 800;
            direction = 1;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    std::cout << std::endl;
    std::cout << "Stopping simulator..." << std::endl;

    can.close();

    return 0;
}
