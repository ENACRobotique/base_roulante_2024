#pragma once

#include "ch.h"
#include "hal.h"
#include "holocontrol.h"
#ifdef __cplusplus
#include "encoders.h"
#include "odometry.h"
 #include "guidance.h"

#define ENCODERS_PERIOD 2
#define ODOM_PERIOD 10
//Nom pour les commandes ( plus lisible que 1,2,...)
#define X 0
#define Y 1
#define THETA 2

extern Encoder enc1;
extern Encoder enc2;
extern Encoder enc3;

extern Odometry odometry;
extern HoloControl holocontrol;
extern Guidance guidance;

#endif


