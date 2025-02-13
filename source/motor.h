#pragma once

#include <hal.h>
#include <stdint.h>

class Motor
{
public:
    Motor(): speed(0), pos(0) {}

    virtual void init() = 0;

    virtual double get_pos() = 0;

    virtual double get_speed() = 0;

    virtual void set_cmd(float cmd) = 0;

    virtual int16_t get_cmd() = 0;

protected:
    // motor speed in RPM
    int16_t speed;

    // motor position, in encoder unit
    int32_t pos;

    // motor command
    int16_t command;
    
};


void motorsStart();

