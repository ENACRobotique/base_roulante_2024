#pragma once
#include "ch.h"
#include "utils.h"
#include "math.h"

class PID {

public:

    PID(): integral(0), prev_err(0), dt(0), kp(1), ki(0), kd(0), max_int_cmd(0) {}
    
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

        if(fabs(error) < 5) {
            error = 0;
        }

        // trapezoidal integration
        integral += (prev_err + error)/2 * dt;
        

        if(ki != 0) {
            //saturate error integral such that the contribution of integrator to the cmd cannot exceed max_int_cmd.
            integral = clamp(-max_int_cmd/ki, integral, max_int_cmd/ki);           
        }

        double derr = (error - prev_err) / dt;
        double cmd = kp*error + ki*integral + kd*derr;

        prev_err = error;
        
        // Deadband du moteur : en dessous de 3.5, il ne tourne pas (frottements, ...)
        // if(cmd > 0.1) {
        //     cmd += 3.5;
        // }
        // else if (cmd < -0.1) {
        //     cmd -= 3.5;
        // }
        cmd = clamp(-100, cmd, 100);

        return cmd;
    }

    void set_gains(double p, double i, double d) {
        kp = p;
        ki = i;
        kd = d;
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
};