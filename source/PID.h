#ifndef PID_H
#define PID_H

#pragma once
#include <math.h>

class PID{

    private:
        double kp=1;
        double ki=0;
        double kd=0;
        double min;
        double max;
        double integral=0;
        double derivative;
        int cmd;

    public :
        double update_cmd(double error, double speed_error, double period);

        double get_kp(){ return this->kp; }
        double get_kd(){ return this->kd; }
        double get_ki(){ return this->ki; }
        double get_integral(){ return this->integral; }
        double get_derivative(){ return this->derivative; }
};

#endif
