#pragma once

#include "ch.h"
#include "hal.h"

#ifdef __cplusplus
#include "encoders.h"
// #include "motors.h"
// #include "OdometryHolo.h"
// #include "HolonomicControl.h"
// #include "guidance.h"

extern Encoder enc1;
extern Encoder enc2;
extern Encoder enc3;

constexpr pwmcnt_t PWM_PERIOD = 250;
extern PWMConfig pwmcfg1;

// extern Motor mot1;
// extern Motor mot2;
// extern Motor mot3;

// extern OdometryHolo odometry;

// extern HolonomicControl control;

// extern Guidance guidance;

#endif


