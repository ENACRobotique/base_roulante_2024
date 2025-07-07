#pragma once

#include "ch.h"
#include <Eigen/Core>
#include "encoders.h"
#include "mot_conf.h"
#include "Position.h"

#define PERIOD_ODOM_REPORT 200  // ms


class Odometry {

private:
    

    VectMot prev_motors_pos;
    Position _position;
    Eigen::Vector3d _speed_r;
    MUTEX_DECL(mut_hgf_pos);

    void send_move(Eigen::Vector3d dpos);


public:
    //Odometry() {}
    void init();

    //Accesseurs
    void set_pos(float x, float y, float theta);

    inline float get_x(void) {return _position.x();}
    inline float get_y(void) {return _position.y();}
    inline float get_theta(void) {return _position.theta();}
    Position get_pos() {return _position;}

    void set_theta(double theta) {_position.set_theta(theta);}

    /**
     * Speed in the robot frame
     */
    Eigen::Vector3d get_speed() {return _speed_r;}


    VectMot get_motors_pos();
    VectMot get_motors_speed();

    void update();
    
    #if defined(BOARD_DC)
    void update_filters();
    #endif

};

