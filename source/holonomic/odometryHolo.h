#pragma once

#include "ch.h"
#include <Eigen/Core>
#include "encoders.h"
#include "mot_conf.h"
#include "Position.h"
#include "odometry.h"

#define PERIOD_ODOM_REPORT 200  // ms


#if DRIVE == DRIVE_HOLO

class odometryHolo: public Odometry {

private:
    

    VectMot prev_motors_pos;
    Position _position;
    Speed _speed_r;
    //Eigen::Vector3d _speed_r;
    MUTEX_DECL(mut_hgf_pos);

    void send_move(Eigen::Vector3d dpos);


public:
    void init() override;
    void set_pos(Position pos) override;
    Position get_pos() override {return _position;}
    Speed get_speed() override {return _speed_r;}
    void set_theta(double theta) override {_position.set_theta(theta);};
    void update() override;

    void send_motor_speed(e::Message<MOTORS_NB>& msg);

    VectMot get_motors_pos();
    VectMot get_motors_speed();
    
    #if defined(BOARD_DC)
    void update_filters();
    #endif

};

#endif
