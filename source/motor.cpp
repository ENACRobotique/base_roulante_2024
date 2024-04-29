#include "motor.h"
#include "stdio.h"


const ioline_t numMoteur[3] = {LINE_MOT1_DIR,LINE_MOT2_DIR,LINE_MOT3_DIR};
const ioline_t pwmChannel[3] = {2, 1, 0};


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

void set_motor(int num_mot,double speed){

    if (speed < 0){
        palWriteLine(numMoteur[num_mot], PAL_LOW); 
    }
    else{
        palWriteLine(numMoteur[num_mot], PAL_HIGH);
    }

    // if (fabs(speed)> MAX_SPEED){
    //    speed = MAX_SPEED;
    // }

    if (fabs(speed)> 100){
       speed = 100;
    }

    pwmcnt_t width = (fabs(speed)/MAX_SPEED)*PWM_PERIOD;

    pwmEnableChannel(&PWMD1, pwmChannel[num_mot], width);


}
