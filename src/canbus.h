#pragma once

#include <cstdint>
#include <string>

/*
 * MaxxECU CAN Simulator
 * Release_001
 *
 * SocketCAN interface for Raspberry Pi
 */

class CanBus
{
public:
    CanBus();
    ~CanBus();

    // Open CAN interface (typically "can0")
    bool open(const std::string& interface);

    // Close interface
    void close();

    // Send one RPM frame
    bool sendRPM(uint16_t rpm);

    // Returns true if CAN socket is open
    bool isOpen() const;

private:
    int socket_fd;

    bool sendFrame(uint32_t id,
                   const uint8_t* data,
                   uint8_t dlc);
};
