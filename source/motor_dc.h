#pragma once
#include "motor.h"
#include "encoders.h"

constexpr double MAX_SPEED = 200;


class MotorDC: public Motor
{
public:

    MotorDC(Encoder* enc, ioline_t dir_pin, pwmchannel_t pwm_channel, bool enc_inverted):
        enc(enc), dir_pin(dir_pin), pwm_channel(pwm_channel), enc_inverted(enc_inverted) {}
    
    virtual void init() override;

    virtual double get_pos() override;

    virtual double get_speed() override;

    virtual void set_cmd(float cmd) override;

    virtual int16_t get_cmd() override;

private:
    Encoder* enc;
    ioline_t dir_pin;
    pwmchannel_t pwm_channel;
    bool enc_inverted;


};
