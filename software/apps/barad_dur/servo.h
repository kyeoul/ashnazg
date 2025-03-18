#pragma once

#include "microbit_v2.h"

#define SERVO_OUT EDGE_P0

void servo_init();

// in degrees from 0 to 180
void servo_write(float angle);

void servo_rotate();

void servo_stop();

float servo_get_angle();