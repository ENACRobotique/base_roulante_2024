#pragma once
#include "ch.h"
#include "utils.h"

class PID {

public:

    PID(): integral(0), prev_err(0), dt(0), kp(1), ki(0), kd(0), max_int_cmd(0), deadzone(0) {}
    
    /**
     * rate in Hz
     * int_max_cmd
    */
    void init(double rate, double int_max_cmd) {
        dt = 1/rate;
        max_int_cmd = int_max_cmd;
    }

    /**
     * dt in seconds
    */
    double update(double error) {

        // if(isnan(integral)) {
        //     chSysHalt("integral NAN");
        // }
        chDbgAssert(!isnan(integral), "integral NAN");

        // trapezoidal integration
        if (abs(error)>deadzone){
            integral += (prev_err + error)/2 * dt;
            
            if(ki != 0) {
                //saturate error integral such that the contribution of integrator to the cmd cannot exceed max_int_cmd.
                integral = clamp(-max_int_cmd/ki, integral, max_int_cmd/ki);
            }
        }


        // Kp*(Pc-P) + Ki*integral(Pc-P) + Kd*d(Pc-p)/dt
        // d(Pc-p)/dt  =  d(Pc)/dt - d(P)/dt  =  Vc - V  =  speed error
        double cmd = kp*error + ki*integral + kd*(error - prev_err)/dt;
        prev_err = error;
        //cmd = clamp(-100, cmd, 100);

        return cmd;
    }

    void set_gains(double p, double i, double d, double dead = 0) {
        kp = p;
        ki = i;
        kd = d;
        deadzone = dead;
    }


private:
    double integral;
    
    // previous error, to compute a better estimate of the integral
    double prev_err;

    double dt;

    double kp;
    double ki;
    double kd;
    double max_int_cmd;
    double deadzone;
};