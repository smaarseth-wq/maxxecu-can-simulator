#pragma once

#include <stdint.h>
#include <string>

class CanBus
{
public:

    CanBus();
    ~CanBus();

    bool open(const std::string& interface = "can0");
    void close();

    bool isOpen() const;

    bool sendFrame(
        uint32_t id,
        const uint8_t* data,
        uint8_t dlc);

    // ===========================
    // MaxxECU Default CAN Frames
    // ===========================

    bool send520(
        uint16_t rpm,
        uint16_t tps,
        uint16_t map,
        uint16_t lambda);

    bool send521(
        int16_t coolant,
        int16_t airtemp,
        uint16_t battery,
        uint16_t oilpressure);

    bool send522(
        uint16_t speed,
        int16_t ignition,
        uint16_t fuelDuty,
        uint16_t ethanol);

private:

    int socket_fd;
};
