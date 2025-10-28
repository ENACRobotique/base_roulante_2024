#include "controlHolo.h"

#ifdef __cplusplus
extern "C" {
#endif
  #include "stdutil.h"
  #include "printf.h"
  #include "utils.h"
#ifdef __cplusplus
}
#endif

#include <cmath>
#include "odometry.h" 
#include "motor.h"
#include "utils.h"
#include "messages.h"
#include "communication.h"
#include "globalVar.h"
#include "ins.h"
#include "mot_conf.h"


/*
 *  |v1|   |-sin(O1)  cos(O1)  R|   |vx|
 *  |v2| = |-sin(O2)  cos(O2)  R| . |vy|
 *  |v3|   |-sin(O3)  cos(O3)  R|   |w|
 *
 *    m  =           D            .   v
 */

// Euclidean speeds into motor speeds: m = Dv


namespace e = enac;

#if DRIVE == DRIVE_HOLO

void ControlHolo::init() {

  motorsStart();

  for(size_t i=0; i<MOTORS_NB; i++) {
    motors[i].set_cmd(0);
  }

  _speed_cons = {0., 0., 0.};
  for (int i=0;i<MOTORS_NB;i++){
    _cmds[i] = {0.};
  }

  _last_setpoint = chSysGetRealtimeCounterX();

  for(int i = 0; i<MOTORS_NB;i++){
    vel_pids[i].init(1000.0/ODOM_PERIOD_MS, 100);
    vel_pids[i].set_gains(0.07, 0.01, 0.008);
    // vel_pids[i].set_gains(0.1, 0.03, -0.001);

    pos_pids[i].init(1000.0/ODOM_PERIOD_MS, 10);
    pos_pids[i].set_gains(2, 0.1, 1);
  }

  _asserve_enabled = true;
  _pos_cascade_enabled = false;
}


/**
 * pos: position setpoint in robot frame
 * speed: speed setpoint in robot frame, relative to earth
*/
void ControlHolo::set_cons(Speed vRobotR)
{  
    _speed_cons = vRobotR.to_eigen();
    _motors_speed_cons = D*(_speed_cons);
    _last_setpoint = chVTGetSystemTime();
}


void ControlHolo::set_vel_pid_gains(double kp, double ki, double kd){
  for(int i=0;i<3;i++){
    vel_pids[i].set_gains(kp, ki, kd);
  } 
}

void ControlHolo::set_pos_pid_gains(double kp, double ki, double kd){
  for(int i=0;i<3;i++){
    pos_pids[i].set_gains(kp, ki, kd);
  } 
}

void ControlHolo::update()
{
  if (chTimeI2MS(chVTTimeElapsedSinceX(_last_setpoint)) > 500) {
    _speed_cons = {0,0,0};
    for(size_t i=0; i<MOTORS_NB; i++) {
      motors[i].set_cmd(0);
    }
  }

  if (!_asserve_enabled) {
    return;
  }


  // Eigen::Vector3d  motors_pos = odometry.get_motors_pos();
  auto motors_speed = odometry.get_motors_speed();

  auto speed_error = _motors_speed_cons - motors_speed;
  

  for(int i=0; i<MOTORS_NB; i++) {
    _cmds[i] = vel_pids[i].update(speed_error[i]);
  }

  for(size_t i=0; i<MOTORS_NB; i++) {
    motors[i].set_cmd(_cmds[i]);
  }

}

void ControlHolo::send_motor_speedcons(e::Message<MOTORS_NB>& msg){
  msg.clear();
  auto& motors_msg = msg.mutable_motors();
  for (int i=0;i<MOTORS_NB;i++){
    motors_msg.add_m(_motors_speed_cons[i]);
  }
  motors_msg.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_SPEED_CONS);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}

void ControlHolo::send_motor_speedcmd(e::Message<MOTORS_NB>& msg){
  msg.clear();
  auto& motors_msg = msg.mutable_motors();
  for (int i=0;i<MOTORS_NB;i++){
    motors_msg.add_m(_cmds[i]);
  }
  motors_msg.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_CMD);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}

#endif