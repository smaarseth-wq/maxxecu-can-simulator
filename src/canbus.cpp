#include "canbus.h"

#include <cstring>
#include <iostream>

#include <unistd.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <net/if.h>

#include <sys/socket.h>

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
        std::cout << "ERROR: Failed to create CAN socket" << std::endl;
        return false;
    }

    struct ifreq ifr;

    std::strncpy(ifr.ifr_name,
                 interface.c_str(),
                 IFNAMSIZ - 1);

    if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0)
    {
        std::cout << "ERROR: Cannot find interface "
                  << interface << std::endl;

        ::close(socket_fd);
        socket_fd = -1;

        return false;
    }

    struct sockaddr_can addr;

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd,
             reinterpret_cast<sockaddr*>(&addr),
             sizeof(addr)) < 0)
    {
        std::cout << "ERROR: Failed to bind CAN socket"
                  << std::endl;

        ::close(socket_fd);
        socket_fd = -1;

        return false;
    }

    std::cout << "CAN interface "
              << interface
              << " opened."
              << std::endl;

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

bool CanBus::sendFrame(uint32_t id,
                       const uint8_t* data,
                       uint8_t dlc)
{
    if (!isOpen())
        return false;

    struct can_frame frame;

    frame.can_id = id;
    frame.can_dlc = dlc;

    std::memset(frame.data, 0, sizeof(frame.data));

    std::memcpy(frame.data,
                data,
                dlc);

    int written = write(socket_fd,
                        &frame,
                        sizeof(frame));

    return written == sizeof(frame);
}

bool CanBus::sendRPM(uint16_t rpm)
{
    uint8_t data[8] = {0};

    data[0] = rpm & 0xFF;
    data[1] = (rpm >> 8) & 0xFF;

    bool ok = sendFrame(0x520,
                        data,
                        8);

    if (ok)
    {
        std::cout
            << "TX 0x520 RPM="
            << rpm
            << std::endl;
    }

    return ok;
}
