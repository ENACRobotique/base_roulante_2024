#pragma once

#include "ch.h"
#include <Eigen/Core>
#include "encoders.h"

#define MOTORS_NB 3
#define PERIOD_ODOM_REPORT 200  // ms

constexpr double ROBOT_RADIUS = 130.5;

extern const Eigen::Matrix<double, 3, 3> D;


class Odometry {

private:
    

    Eigen::Vector3d prev_motors_pos;
    Eigen::Vector3d _position;
    Eigen::Vector3d _speed_r;
    MUTEX_DECL(mut_hgf_pos);

    void send_move(Eigen::Vector3d dpos);


public:
    //Odometry() {}
    void init();

    //Accesseurs
    void set_pos(double x, double y, double theta);

    double get_x(void) {return _position[0];}
    double get_y(void) {return _position[1];}
    double get_theta(void) {return _position[2];}
    Eigen::Vector3d get_pos() {return _position;}

    void set_theta(double theta) {_position[2] = theta;}

    /**
     * Speed in the robot frame
     */
    Eigen::Vector3d get_speed() {return _speed_r;}


    Eigen::Vector3d get_motors_pos();
    Eigen::Vector3d get_motors_speed();

    void update();
    
    #if defined(BOARD_DC)
    void update_filters();
    #endif

};

