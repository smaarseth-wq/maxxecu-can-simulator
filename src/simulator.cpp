#include "simulator.h"

Simulator::Simulator()
{
    rpm = 800;
    rpmDirection = 100;

    throttle = 0;
    map = 35;
    lambda = 1000;          // 1.000

    coolantTemp = 90;
    airTemp = 25;

    batteryVoltage = 142;   // 14.2V
    oilPressure = 200;

    vehicleSpeed = 0;
    ignition = 15;

    fuelDuty = 0;
    ethanol = 0;
}

void Simulator::update()
{
    updateRPM();
    updateSensors();
}

void Simulator::updateRPM()
{
    rpm += rpmDirection;

    if(rpm >= 7000)
    {
        rpm = 7000;
        rpmDirection = -100;
    }

    if(rpm <= 800)
    {
        rpm = 800;
        rpmDirection = 100;
    }
}

void Simulator::updateSensors()
{
    //
    // TPS følger RPM
    //

    throttle =
        (rpm - 800) * 100 / (7000 - 800);

    //
    // MAP 35 -> 220 kPa
    //

    map =
        35 +
        (throttle * 185) / 100;

    //
    // Lambda
    //

    if(throttle < 30)
        lambda = 1000;
    else if(throttle < 70)
        lambda = 970;
    else
        lambda = 920;

    //
    // Oljetrykk
    //

    oilPressure =
        150 +
        (rpm / 40);

    //
    // Hastighet
    //

    vehicleSpeed =
        rpm / 40;

    //
    // Fuel duty
    //

    fuelDuty =
        throttle / 2;
}

void Simulator::send(CanBus& can)
{
    can.send520(
        rpm,
        throttle,
        map,
        lambda);

    can.send521(
        coolantTemp,
        airTemp,
        batteryVoltage,
        oilPressure);

    can.send522(
        vehicleSpeed,
        ignition,
        fuelDuty,
        ethanol);
}
