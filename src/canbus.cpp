#include "canbus.h"

#include <cstring>
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <net/if.h>

CanBus::CanBus()
{
    socket_fd = -1;
}

CanBus::~CanBus()
{
    close();
}

bool CanBus::open(const std::string& interface)
{
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (socket_fd < 0)
    {
        perror("socket");
        return false;
    }

    struct ifreq ifr {};

    std::strncpy(
        ifr.ifr_name,
        interface.c_str(),
        IFNAMSIZ - 1);

    if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("SIOCGIFINDEX");

        ::close(socket_fd);
        socket_fd = -1;

        return false;
    }

    sockaddr_can addr {};

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(
            socket_fd,
            reinterpret_cast<sockaddr*>(&addr),
            sizeof(addr)) < 0)
    {
        perror("bind");

        ::close(socket_fd);
        socket_fd = -1;

        return false;
    }

    std::cout << "CAN interface : " << interface << std::endl;
    std::cout << "Socket opened." << std::endl;

    return true;
}

void CanBus::close()
{
    if (socket_fd >= 0)
    {
        ::close(socket_fd);
        socket_fd = -1;
    }
}

bool CanBus::isOpen() const
{
    return socket_fd >= 0;
}

bool CanBus::sendFrame(
    uint32_t id,
    const uint8_t* data,
    uint8_t dlc)
{
    if (!isOpen())
        return false;

    struct can_frame frame {};

    frame.can_id = id;
    frame.len = dlc;

    std::memcpy(
        frame.data,
        data,
        dlc);

    int ret = write(
        socket_fd,
        &frame,
        sizeof(frame));

    if (ret != sizeof(frame))
    {
        perror("write");
        return false;
    }

    return true;
}

bool CanBus::send520(
    uint16_t rpm,
    uint16_t tps,
    uint16_t map,
    uint16_t lambda)
{
    uint8_t data[8];

    data[0] = rpm & 0xFF;
    data[1] = rpm >> 8;

    data[2] = tps & 0xFF;
    data[3] = tps >> 8;

    data[4] = map & 0xFF;
    data[5] = map >> 8;

    data[6] = lambda & 0xFF;
    data[7] = lambda >> 8;

    return sendFrame(0x520, data, 8);
}

bool CanBus::send521(
    int16_t coolant,
    int16_t airtemp,
    uint16_t battery,
    uint16_t oilpressure)
{
    uint8_t data[8];

    data[0] = coolant & 0xFF;
    data[1] = coolant >> 8;

    data[2] = airtemp & 0xFF;
    data[3] = airtemp >> 8;

    data[4] = battery & 0xFF;
    data[5] = battery >> 8;

    data[6] = oilpressure & 0xFF;
    data[7] = oilpressure >> 8;

    return sendFrame(0x521, data, 8);
}

bool CanBus::send522(
    uint16_t speed,
    int16_t ignition,
    uint16_t fuelDuty,
    uint16_t ethanol)
{
    uint8_t data[8];

    data[0] = speed & 0xFF;
    data[1] = speed >> 8;

    data[2] = ignition & 0xFF;
    data[3] = ignition >> 8;

    data[4] = fuelDuty & 0xFF;
    data[5] = fuelDuty >> 8;

    data[6] = ethanol & 0xFF;
    data[7] = ethanol >> 8;

    return sendFrame(0x522, data, 8);
}
