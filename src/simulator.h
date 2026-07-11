#pragma once

#include "canbus.h"

#include <stdint.h>

class Simulator
{
public:

    Simulator();

    void update();
    void send(CanBus& can);

private:

    // ==========================
    // Motor
    // ==========================

    uint16_t rpm;
    int rpmDirection;

    // ==========================
    // Fast1 (0x520)
    // ==========================

    uint16_t throttle;
    uint16_t map;
    uint16_t lambda;

    // ==========================
    // Fast2 (0x521)
    // ==========================

    int16_t coolantTemp;
    int16_t airTemp;

    uint16_t batteryVoltage;
    uint16_t oilPressure;

    // ==========================
    // Fast3 (0x522)
    // ==========================

    uint16_t vehicleSpeed;
    int16_t ignition;

    uint16_t fuelDuty;
    uint16_t ethanol;

    void updateRPM();
    void updateSensors();
};
