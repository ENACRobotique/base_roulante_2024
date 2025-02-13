#pragma once
#include "motor.h"

#include <ch.h>
#include <hal.h>

// distance per motor turn
constexpr double MM_PER_TURN = 188.49555921538757 / 36.0;

// increments per motor turn
constexpr double INC_PER_TURN = 8192;


class MotorCAN: public Motor
{
 
public:
    MotorCAN(): angle(0), torque(0) {}

    virtual void init() override {};

    virtual double get_pos() override { return -pos*MM_PER_TURN/INC_PER_TURN; }

    virtual double get_speed() override { return -speed*MM_PER_TURN/60.0; }

    virtual void set_cmd(float cmd) override;

    virtual int16_t get_cmd() override;

    void set_status(int16_t new_angle, int16_t new_speed, int16_t new_torque);

private:
    // motor status
    int16_t angle;

    int16_t torque;

};