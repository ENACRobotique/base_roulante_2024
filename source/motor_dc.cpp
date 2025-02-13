#include "motor_dc.h"
#include "stdio.h"
#include "motor_dc.h"
#include "encoders.h"
#include <hal.h>

#if defined(BOARD_DC)


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


void motorsStart() {
    pwmStart(&PWMD1, &pwmcfg1);
}

void MotorDC::init()
{
    enc->init(enc_inverted);
}

double MotorDC::get_pos()
{
    return enc->get_pos();
}

double MotorDC::get_speed()
{
    return enc->get_speed();
}

void MotorDC::set_cmd(float cmd)
{
    if (cmd < 0){
        palWriteLine(dir_pin, PAL_LOW); 
    }
    else{
        palWriteLine(dir_pin, PAL_HIGH);
    }


    if (fabs(cmd)> 100){
       cmd = 100;
    }

    pwmcnt_t width = (fabs(cmd)/MAX_SPEED)*PWM_PERIOD;

    pwmEnableChannel(&PWMD1, pwm_channel, width);

    command = cmd;

}

int16_t MotorDC::get_cmd()
{
    return command;
}

#endif