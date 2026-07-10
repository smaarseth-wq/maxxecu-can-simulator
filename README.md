# MaxxECU CAN Simulator

Release_001

Simple CAN simulator for Raspberry Pi using SocketCAN.

Current functionality:

- Opens CAN interface `can0`
- Sends RPM on CAN ID `0x520`
- RPM sweeps automatically from 800 RPM to 8000 RPM
- Sends one CAN frame every 20 ms (50 Hz)

---

## Requirements

Raspberry Pi OS

SocketCAN

can-utils

Install can-utils

```bash
sudo apt update
sudo apt install can-utils
```

---

## Bring CAN interface up

```bash
sudo ip link set can0 down
sudo ip link set can0 up type can bitrate 500000
```

Check interface

```bash
ip link show can0
```

---

## Build

```bash
mkdir build
cd build

cmake ..
make -j4
```

---

## Run

```bash
./maxxecu_can_simulator
```

You should see something similar to:

```
CAN interface : can0

Socket opened successfully.

TX 0x520 RPM = 800
TX 0x520 RPM = 900
TX 0x520 RPM = 1000
...
```

Stop with

```
CTRL+C
```

---

## Release_001

Only RPM is transmitted.

Next release will add more MaxxECU signals.
