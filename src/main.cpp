#include "canbus.h"
#include "simulator.h"

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
    std::cout << "==========================================" << std::endl;
    std::cout << " MaxxECU CAN Simulator Release_002" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;

    CanBus can;

    std::cout << "Opening CAN interface..." << std::endl;

    if (!can.open("can0"))
    {
        std::cout << "ERROR: Failed to open CAN interface." << std::endl;
        return -1;
    }

    std::cout << "CAN interface ready." << std::endl;

    Simulator simulator;

    auto lastFast20 = std::chrono::steady_clock::now();
    auto lastFast100 = std::chrono::steady_clock::now();

    while (running)
    {
        auto now = std::chrono::steady_clock::now();

        //
        // 20 ms task
        //
        if (now - lastFast20 >= std::chrono::milliseconds(20))
        {
            simulator.update();
            simulator.send(can);

            lastFast20 = now;
        }

        //
        // Future 100 ms messages
        //
        if (now - lastFast100 >= std::chrono::milliseconds(100))
        {
            // send 0x523
            // send 0x524
            // send 0x525

            lastFast100 = now;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(1));
    }

    std::cout << std::endl;
    std::cout << "Stopping simulator..." << std::endl;

    can.close();

    return 0;
}
