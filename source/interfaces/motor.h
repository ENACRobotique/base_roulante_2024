#pragma once

#include <hal.h>
#include <stdint.h>

class Motor
{
public:
    Motor(): speed(0), filter_speed(0), pos(0) {}

    virtual void init() = 0;

    virtual double get_pos() = 0;

    virtual double get_speed() = 0;

    virtual void set_cmd(double cmd) = 0;

    virtual double get_cmd() = 0;

protected:
    // motor speed in RPM
    int16_t speed;

    double filter_speed;

    // motor position, in encoder unit
    int32_t pos;

    // motor command
    double command;
    
};


void motorsStart();

