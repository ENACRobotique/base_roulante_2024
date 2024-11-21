#include "globalVar.h"
#include "hal.h"

/*
 * Encoders
 */
constexpr double INC_PER_MM = 15.556;

Encoder enc1(STM32_TIM3, INC_PER_MM);
Encoder enc2(STM32_TIM2, INC_PER_MM);
Encoder enc3(STM32_TIM4, INC_PER_MM);

Odometry odometry;
HoloControl holocontrol;
Guidance guidance;
SystemManager systemmanager;
