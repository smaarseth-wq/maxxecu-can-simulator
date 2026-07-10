#pragma once

#include <stdint.h>

bool can_init(const char *iface);

bool send_rpm(uint16_t rpm);

void can_close();
