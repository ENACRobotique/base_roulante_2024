#include "globalVar.h"
#include "hal.h"
#include "motor_dc.h"


#if defined(BOARD_DC)
constexpr double INC_PER_MM = 15.556;

Encoder enc1(STM32_TIM3, INC_PER_MM);
Encoder enc2(STM32_TIM2, INC_PER_MM);
Encoder enc3(STM32_TIM4, INC_PER_MM);

MotorDC mot1(&enc1, LINE_MOT1_DIR, 2, true);
MotorDC mot2(&enc2, LINE_MOT2_DIR, 1, false);
MotorDC mot3(&enc3, LINE_MOT3_DIR, 0, false);
#elif defined(BOARD_CAN)
std::array<MotorCAN, 4> motors;
#endif

Odometry odometry;
HoloControl holocontrol;
Guidance guidance;
SystemManager systemmanager;
