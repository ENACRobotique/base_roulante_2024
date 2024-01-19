#include "motor.h"
#include "stdio.h"


ioline_t numMoteur[3] = {LINE_MOT1_DIR,LINE_MOT2_DIR,LINE_MOT3_DIR};
ioline_t pwmChannel[3] = {2, 1, 0};
void set_motor(int num_mot,double speed){

    if (speed < 0){
        palWriteLine(numMoteur[num_mot], PAL_LOW); // verifier le sens de rotation
    }
    else{
        palWriteLine(numMoteur[num_mot], PAL_HIGH);
    }

    if (fabs(speed)> 80){
        speed = 80;
    }
    pwmcnt_t width = (fabs(speed)/100)*PWM_PERIOD;

    

    pwmEnableChannel(&PWMD1, pwmChannel[num_mot], width);


}
