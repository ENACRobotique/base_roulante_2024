#pragma once

#include "hal.h"
#include <math.h>
#include "globalVar.h"

void motorsStart();
void set_motor(int num_pot,double speed);

constexpr double MAX_SPEED = 200;
