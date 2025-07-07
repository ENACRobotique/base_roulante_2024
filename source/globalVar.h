#pragma once

#include "ch.h"
#include "hal.h"
#include "holocontrol.h"
#ifdef __cplusplus
#include "encoders.h"
#include "odometry.h"
#include "guidance.h"

#if defined(BOARD_DC)
#include "motor_dc.h"
#elif defined(BOARD_CAN)
#include "motor_can.h"
#endif


#define ENCODERS_PERIOD 2
#define ODOM_PERIOD 20 //10
//Nom pour les commandes ( plus lisible que 1,2,...)
#define X 0
#define Y 1
#define THETA 2


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

extern Odometry odometry;
extern HoloControl holocontrol;
extern Guidance guidance;



#endif


