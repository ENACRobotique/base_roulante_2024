#pragma once
#include "utils.h"
#include <hal.h>
#include <ch.h>
#include <Eigen/Core>
#include "PID.h"
#include "odometry.h"
#include "mot_conf.h"

class HoloControl {

private:

    Eigen::Matrix<double, MOTORS_NB, 1> _pos_cons;
    
    // vitesse de consigne du robot (vx, vy, vtheta) en repere robot
    Eigen::Vector3d _speed_cons;

    bool _pos_cascade_enabled;
    bool _asserve_enabled;

    PID pos_pids[MOTORS_NB];
    PID vel_pids[MOTORS_NB];

    // keep commands for logging
    Eigen::Matrix<double, MOTORS_NB, 1> _cmds;

    systime_t _last_setpoint;


public:

    HoloControl(){};
    void init();

    void set_cons(const Eigen::Vector3d& posRobotR, const Eigen::Vector3d& vRobotR);
    void update();
    void set_vel_pid_gains(double kp, double ki, double kd);
    void set_pos_pid_gains(double kp, double ki, double kd);
    
    //Adding position asserve in Cascade asserve (see update())
    void enable_position_control(bool en) {_pos_cascade_enabled = en;}
    void enable_asserve(bool enable){_asserve_enabled = enable;}

    Eigen::Matrix<double, MOTORS_NB, 1> get_cmds() { return _cmds;}
    Eigen::Matrix<double, MOTORS_NB, 1> get_pos_cons() { return _pos_cons;}
    Eigen::Vector3d get_speed_cons() { return _speed_cons;}


};