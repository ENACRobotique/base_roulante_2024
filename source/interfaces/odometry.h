#pragma once

#include "ch.h"
#include <Eigen/Core>
#include "encoders.h"
#include "mot_conf.h"
#include "Position.h"
#include "Speed.h"

namespace e = enac;

class Odometry {

private:
public:
    virtual void init()=0;

    //Accesseurs
    virtual void set_pos(Position pos)=0;

    /**
     * @return robot position in table frame
     */
    virtual Position get_pos()=0;

    /**
     * @return robot speed in robot frame, relative to the earth
     */
    virtual Speed get_speed()=0;

    virtual void set_theta(double theta)=0;

    virtual void update()=0;

    virtual void send_motor_speed(e::Message<MOTORS_NB>& msg)=0;
    
};

