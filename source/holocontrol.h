#pragma once
#include "utils.h"
#include <hal.h>
#include <Eigen/Core>
#include "PID.h"
#include "odometry.h"

#define SETPOINT_VALIDITY 1000  //ms

class HoloControl {

private:

    Eigen::Vector3d _pos_cons;
    Eigen::Vector3d _speed_cons;

    PID pids[MOTORS_NB];

    // keep commands for logging
    Eigen::Vector3d _cmds;


public:

    HoloControl(){};
    void init();

    void set_cons(Eigen::Vector3d posRobotR, Eigen::Vector3d vRobotR);
    void update();

    Eigen::Vector3d get_cmds() { return _cmds;}
    Eigen::Vector3d get_pos_cons() { return _pos_cons;}
    Eigen::Vector3d get_speed_cons() { return _speed_cons;}


};