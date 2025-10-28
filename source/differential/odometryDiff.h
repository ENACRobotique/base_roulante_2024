#pragma once

#include "ch.h"
#include <Eigen/Core>
#include "encoders.h"
#include "mot_conf.h"
#include "Position.h"
#include "odometry.h"

#define PERIOD_ODOM_REPORT 200  // ms

namespace e = enac;

#if DRIVE == DRIVE_DIFF

class OdometryDiff: public Odometry {

private:

    Position _position;
    Speed _speed_r;
    double mot_gauche_prev_pos;
    double mot_droit_prev_pos;
    double robot_vit_d;
    double robot_vit_ang;
    
    
    //void send_move();


public:
    void init() override;
    void set_pos(Position pos) override;
    Position get_pos() override {return _position;}
    Speed get_speed() override {return _speed_r;}
    void set_theta(double theta) override {_position.set_theta(theta);};
    void update() override;

    double get_vit_d() {return robot_vit_d;}
    double get_vit_ang() {return robot_vit_ang;}

    void send_move(Position dpos);
    void send_motor_speed(e::Message<MOTORS_NB>& msg) override;

    #if defined(BOARD_DC)
    void update_filters();
    #endif

};

#endif
