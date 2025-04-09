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
    
    // vitesse de consigne du robot (vx, vy, vtheta) en repere robot
    Eigen::Vector3d _speed_cons;

    bool _pos_cascade_enabled;
    bool _asserve_enabled;

    PID pos_pids[MOTORS_NB];
    PID vel_pids[MOTORS_NB];

    PID speedR_pids[3];

    // keep commands for logging
    Eigen::Vector3d _cmds;

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

    Eigen::Vector3d get_cmds() { return _cmds;}
    Eigen::Vector3d get_pos_cons() { return _pos_cons;}
    Eigen::Vector3d get_speed_cons() { return _speed_cons;}


};