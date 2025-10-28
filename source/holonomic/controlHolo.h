#pragma once
#include "utils.h"
#include <hal.h>
#include <ch.h>
#include <Eigen/Core>
#include "PID.h"
#include "odometryHolo.h"
#include "mot_conf.h"
#include "control.h"

#if DRIVE == DRIVE_HOLO

class ControlHolo: public Control {

private:

    VectMot _motors_pos_cons;
    VectMot _motors_speed_cons;
    
    // vitesse de consigne du robot (vx, vy, vtheta) en repere robot
    Eigen::Vector3d _speed_cons;

    bool _pos_cascade_enabled;
    bool _asserve_enabled;

    PID pos_pids[MOTORS_NB];
    PID vel_pids[MOTORS_NB];

    // keep commands for logging
    VectMot _cmds;

    systime_t _last_setpoint;


public:

    ControlHolo(){};
    void init() override;
    void update() override;
    // void send_motor_speedcons(e::Message<MOTORS_NB>& msg) override;
    // void send_motor_speedcmd(e::Message<MOTORS_NB>& msg) override;

    void set_cons(Speed vRobotR);

    void set_vel_pid_gains(double kp, double ki, double kd);
    void set_pos_pid_gains(double kp, double ki, double kd);
    
    //Adding position asserve in Cascade asserve (see update())
    void enable_position_control(bool en) {_pos_cascade_enabled = en;}
    void enable_asserve(bool enable){_asserve_enabled = enable;}

    


    VectMot get_motors_cmds() { return _cmds;}
    VectMot get_motors_pos_cons() { return _motors_pos_cons;}
    VectMot get_motors_speed_cons() { return _motors_speed_cons;}
    Eigen::Vector3d get_speed_cons() { return _speed_cons;}

    void send_motor_speedcons(e::Message<MOTORS_NB>& msg);
    void send_motor_speedcmd(e::Message<MOTORS_NB>& msg);


};

#endif