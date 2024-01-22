#include "globalVar.h"
#include "hal.h"

/*
 * Encoders
 */
constexpr double INC_PER_MM = 17.753023791070714;

Encoder enc1(STM32_TIM3, INC_PER_MM);
Encoder enc2(STM32_TIM2, INC_PER_MM);
Encoder enc3(STM32_TIM4, INC_PER_MM);

Odometry odometry;

