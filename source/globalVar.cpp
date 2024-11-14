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


void StateHolder::set_control(uint32_t asserve)
{
    _asserve_state = asserve;
    if (asserve & (uint32_t)protoduck::System::AsservFlags::ASSERV_SPEED)
    {
        holocontrol.enable_position_control(false);
    }
    
    if (asserve & (uint32_t)protoduck::System::AsservFlags::ASSERV_POS)
    {
        holocontrol.enable_position_control(true);
    }
    
    }
