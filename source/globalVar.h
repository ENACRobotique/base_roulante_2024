#pragma once

#include "ch.h"
#include "hal.h"
#include "holocontrol.h"
#ifdef __cplusplus
#include "encoders.h"
#include "odometry.h"
// #include "HolonomicControl.h"
// #include "guidance.h"

#define ENCODERS_PERIOD 2
#define ODOM_PERIOD 5

extern Encoder enc1;
extern Encoder enc2;
extern Encoder enc3;

extern Odometry odometry;
extern HoloControl holocontrol;

// extern HolonomicControl control;

// extern Guidance guidance;

#endif


