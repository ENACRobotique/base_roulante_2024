
#include <ch.h>
#include <hal.h>
#include "encoders.h"
#include <cstdint>
#include "stm32_tim.h"

constexpr float POS_ALPHA0 = 2.4f;
constexpr float POS_ALPHA1 = 2.08f;
constexpr float POS_ALPHA2 = 0.64f;
constexpr float POS_EPS = 0.04f; // 0.02

std::array<double, 3> alphas = {POS_ALPHA0, POS_ALPHA1, POS_ALPHA2};


Encoder::Encoder(stm32_tim_t* tim, double inc_to_mm):
tim(tim), offset(0),
lower_half(true), inc_to_mm(inc_to_mm)
{
  pos_filter.init(alphas, POS_EPS, 40);
}

void Encoder::init(bool inverted) {
  if(tim == STM32_TIM2) {
    // 32 bits
    rccEnableTIM2(false);
    rccResetTIM2();
  }
  else if(tim == STM32_TIM3) {
    // 16 bits
    rccEnableTIM3(false);
    rccResetTIM3();
  }
  else if(tim == STM32_TIM4) {
    // 16 bits
    rccEnableTIM4(false);
    rccResetTIM4();
  }
  else {
    chSysHalt("Timer not supported");
  }

  // Encoder mode 3: Counter counts up/down on both TI1FP1 and TI2FP2 edges
  tim->SMCR =  TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
  
  if(inverted) {
    tim->CCER = TIM_CCER_CC1P;
  } else {
    tim->CCER = 0;
  }
  
  // count from 0-ARR or ARR-0. Limit to 16 bits so it works the same on all timers
  tim->ARR = 0xFFFF;

  // f_DTS/16, N=8, IC1->TI1, IC2->TI2   // TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
  tim->CCMR1 = 0xC1C1;

  tim->CNT = offset;      // Initialize counter
  tim->EGR = 1;           // generate an update event to trigger configuration update
  tim->CR1 = TIM_CR1_CEN; // Enable the counter
}

int32_t Encoder::get_value() {
  uint32_t counter_val = tim->CNT;

  if(tim->SR & TIM_SR_UIF) {  //an overflow/underflow happend 
    if(counter_val <= 0x7FFF && !lower_half) {
      // the counter is in the lower half, and was previously in the upper half: overflow
      offset += 1<<16;
    } else if(counter_val > 0x7FFF && lower_half) {
      // the counter is in the upper half, and was previously in the lower half: underflow
      offset -= 1<<16;
    }
    // if it's not one of the abover cases, an overflow/underflow happened,
    // but the counter did the opposite underflow/overflow,
    // so nothing needs to be done.

    tim->SR = 0;   // reset interrupt flag
  }
  
  if(counter_val <= 0x7FFF) {
    lower_half = true;
  }
  else {
    lower_half = false;
  }

  return static_cast<int32_t>(counter_val) + offset;
}

void Encoder::update_filter()
{
  pos_filter.process(get_value() / inc_to_mm);
}
