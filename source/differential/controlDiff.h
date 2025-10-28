#pragma once
#include "utils.h"
#include <hal.h>
#include <ch.h>
#include <Eigen/Core>
#include "PID.h"
#include "odometryDiff.h"
#include "mot_conf.h"
#include "control.h"

namespace e = enac;

class ControlDiff : public Control {

private:

    double robot_vit_d_cons;
    double robot_vit_ang_cons;

    double cmd_d;
    double cmd_ang;

    PID d_pid;
    PID ang_pid;

    systime_t _last_setpoint;


public:

    ControlDiff(){};
    void init();
    void set_cons(Speed vRobotR);
    void update();

    void stop_motors();
    void set_vit_d_pid_gains(double kp, double ki, double kd);
    void set_vit_ang_pid_gains(double kp, double ki, double kd);
    
    //Adding position asserve in Cascade asserve (see update())
    // void enable_position_control(bool en) {_pos_cascade_enabled = en;}
    // void enable_asserve(bool enable){_asserve_enabled = enable;}

    // VectMot get_motors_cmds() { return _cmds;}
    // VectMot get_motors_pos_cons() { return _motors_pos_cons;}

    void send_motor_speedcons(e::Message<MOTORS_NB>& msg);
    void send_motor_speedcmd(e::Message<MOTORS_NB>& msg);

    //VectMot get_motors_speed_cons() { return VectMot(mot_droit_vit_cmd,mot_gauche_vit_cmd,0,0);}
    // VectMot get_motors_speed_cons() { return _motors_speed_cons;}
    // Eigen::Vector3d get_speed_cons() { return _speed_cons;}


};