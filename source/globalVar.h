#pragma once

#include "ch.h"
#include "hal.h"
#ifdef __cplusplus
#include "encoders.h"
#include "mot_conf.h"
#include "ekf.h"

#if DRIVE == DRIVE_HOLO
#include "odometryHolo.h"
#include "controlHolo.h"
#include "guidanceHolo.h"
extern odometryHolo odometry;
extern ControlHolo control;
extern GuidanceHolo guidance;
#elif DRIVE == DRIVE_DIFF
#include "odometryDiff.h"
#include "controlDiff.h"
#include "guidanceDiff.h"
extern OdometryDiff odometry;
extern ControlDiff control;
extern GuidanceDiff guidance;
#endif

#if defined(BOARD_DC)
#include "motor_dc.h"
#define ENCODERS_PERIOD 2
#elif defined(BOARD_CAN)
#include "motor_can.h"
#endif


constexpr int ODOM_PERIOD_MS = 20;

//#define ODOM_PERIOD 20


#if defined(BOARD_DC)
extern Encoder enc1;
extern Encoder enc2;
extern Encoder enc3;

extern MotorDC mot1;
extern MotorDC mot2;
extern MotorDC mot3;
#elif defined(BOARD_CAN)
extern std::array<MotorCAN, 4> motors;
#endif


extern EKF ekf;


#endif


