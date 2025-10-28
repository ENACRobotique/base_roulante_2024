#pragma once
#include "utils.h"
#include <hal.h>
#include <ch.h>
#include <Eigen/Core>
#include "PID.h"
#include "odometryHolo.h"
#include "mot_conf.h"

class Control {

private:
public:
    virtual void init()=0;
    virtual void update()=0;

    virtual void send_motor_speedcons(e::Message<MOTORS_NB>& msg)=0;
    virtual void send_motor_speedcmd(e::Message<MOTORS_NB>& msg)=0;

};