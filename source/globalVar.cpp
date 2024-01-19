#include "globalVar.h"
#include "hal.h"

/*
 * Encoders
 */
constexpr double INC_PER_MM = 17.753023791070714;

Encoder enc1(STM32_TIM3, INC_PER_MM);
Encoder enc2(STM32_TIM2, INC_PER_MM);
Encoder enc3(STM32_TIM4, INC_PER_MM);


constexpr uint32_t PWM_FREQ = 50000;
constexpr pwmcnt_t PWM_PERIOD = 250;

PWMConfig pwmcfg1 = {
  .frequency = PWM_FREQ,
  .period = PWM_PERIOD,
  .callback = NULL,
  .channels = {
    {                                                               //ch1
      .mode = PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,
      .callback = NULL,
    },
    {                                                               //ch2
      .mode = PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,
      .callback = NULL
    },
    {                                                               //ch3
      .mode = PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,
      .callback = NULL
    },
    {                                                               //ch4
      .mode = PWM_OUTPUT_DISABLED,
      .callback = NULL
    },
  },
  .cr2 = 0,
  .bdtr = 0,
  .dier = 0
};

