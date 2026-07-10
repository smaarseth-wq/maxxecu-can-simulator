#include "canbus.h"

#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    if(!can_init("can0"))
    {
        std::cout << "CAN init failed\n";
        return 1;
    }

    std::cout << "CAN started\n";

    uint16_t rpm = 800;

    while(true)
    {
        send_rpm(rpm);

        std::cout << "RPM " << rpm << std::endl;

        rpm += 100;

        if(rpm > 8000)
            rpm = 800;

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
