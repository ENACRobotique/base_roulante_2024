#pragma once
#include "utils.h"
#include <hal.h>
#include <ch.h>
#include <Eigen/Core>
#include "PID.h"
#include "odometry.h"


class HoloControl {

private:

    Eigen::Vector3d _pos_cons;
    Eigen::Vector3d _speed_cons;

    bool pos_cascade_enabled;

    PID pos_pids[MOTORS_NB];
    PID vel_pids[MOTORS_NB];

    // keep commands for logging
    Eigen::Vector3d _cmds;

    systime_t last_setpoint;


public:

    HoloControl(){};
    void init();

    void set_cons(const Eigen::Vector3d& posRobotR, const Eigen::Vector3d& vRobotR);
    void update();
    void set_vel_pid_gains(double kp, double ki, double kd);
    void set_pos_pid_gains(double kp, double ki, double kd);
    
    //Adding position asserve in Cascade asserve (see update())
    void enable_position_control(bool en) {pos_cascade_enabled = en;}

    Eigen::Vector3d get_cmds() { return _cmds;}
    Eigen::Vector3d get_pos_cons() { return _pos_cons;}
    Eigen::Vector3d get_speed_cons() { return _speed_cons;}


};