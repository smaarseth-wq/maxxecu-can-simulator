#include "canbus.h"

#include <iostream>
#include <cstring>

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

    std::strncpy(ifr.ifr_name,
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

    if (bind(socket_fd,
             reinterpret_cast<sockaddr*>(&addr),
             sizeof(addr)) < 0)
    {
        perror("bind");

        ::close(socket_fd);
        socket_fd = -1;

        return false;
    }

    std::cout
        << "--------------------------------------"
        << std::endl;

    std::cout
        << "CAN interface : "
        << interface
        << std::endl;

    std::cout
        << "Socket opened successfully."
        << std::endl;

    std::cout
        << "--------------------------------------"
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

    can_frame frame {};

    frame.can_id = id;
    frame.len = dlc;

    std::memcpy(frame.data,
                data,
                dlc);

    ssize_t result =
        send(socket_fd,
             &frame,
             sizeof(frame),
             0);

    if (result != sizeof(frame))
    {
        perror("send");
        return false;
    }

    return true;
}

bool CanBus::sendRPM(uint16_t rpm)
{
    uint8_t data[8] = {0};

    /*
        MaxxECU test frame

        ID = 0x520

        Byte0 = RPM low
        Byte1 = RPM high
    */

    data[0] = rpm & 0xFF;
    data[1] = (rpm >> 8) & 0xFF;

    bool ok =
        sendFrame(
            0x520,
            data,
            8);

    if (ok)
    {
        std::cout
            << "TX 0x520   RPM = "
            << rpm
            << std::endl;
    }

    return ok;
}
