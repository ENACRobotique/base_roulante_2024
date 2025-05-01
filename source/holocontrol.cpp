#include "holocontrol.h"
#ifdef __cplusplus
extern "C" {
#endif
  
  #include <ch.h>
  #include <hal.h>
   
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


/*
 *  |v1|   |-sin(O1)  cos(O1)  R|   |vx|
 *  |v2| = |-sin(O2)  cos(O2)  R| . |vy|
 *  |v3|   |-sin(O3)  cos(O3)  R|   |w|
 *
 *    m  =           D            .   v
 */

// Euclidean speeds into motor speeds: m = Dv


using namespace protoduck;

void HoloControl::init() {

  motorsStart();

  mot1.set_cmd(0);
  mot2.set_cmd(0);
  mot3.set_cmd(0);

  _pos_cons = {0., 0., 0.};
  _speed_cons = {0., 0., 0.};
  _cmds = {0., 0., 0.};

  _last_setpoint = chSysGetRealtimeCounterX();

  for(int i = 0; i<3;i++){
    vel_pids[i].init(ODOM_PERIOD, 100);
    vel_pids[i].set_gains(0.1, 0.03, -0.001);

    pos_pids[i].init(ODOM_PERIOD, 10);
    pos_pids[i].set_gains(2, 0.1, 1);

    speedR_pids[i].init(ODOM_PERIOD, 100);
  }
  // speedR_pids[0].set_gains(0, 0.1, 0);
  // speedR_pids[1].set_gains(0, 0.1, 0);
  // speedR_pids[2].set_gains(0, 0.5, 0);
  speedR_pids[0].set_gains(0, 0.0, 0);
  speedR_pids[1].set_gains(0, 0.0, 0);
  speedR_pids[2].set_gains(0, 0.0, 0);

  _asserve_enabled = true;
  _pos_cascade_enabled = false;
}


/**
 * pos: position setpoint in robot frame
 * speed: speed setpoint in robot frame, relative to earth
*/
void HoloControl::set_cons(const Eigen::Vector3d& posRobotR, const Eigen::Vector3d& vRobotR)
{  
    _pos_cons = (D * posRobotR) + odometry.get_motors_pos();
    _speed_cons = vRobotR;
    _last_setpoint = chVTGetSystemTime();
}


void HoloControl::set_vel_pid_gains(double kp, double ki, double kd){
  for(int i=0;i<3;i++){
    vel_pids[i].set_gains(kp, ki, kd);
  } 
}

void HoloControl::set_pos_pid_gains(double kp, double ki, double kd){
  for(int i=0;i<3;i++){
    pos_pids[i].set_gains(kp, ki, kd);
  } 
}

void HoloControl::update()
{
  if (chTimeI2MS(chVTTimeElapsedSinceX(_last_setpoint)) > 500) {
    _speed_cons = {0,0,0};
    mot1.set_cmd(0);
    mot2.set_cmd(0);
    mot3.set_cmd(0);
  }

  if (!_asserve_enabled) {
    return;
  }


  Eigen::Vector3d speedR = odometry.get_speed();
  speedR[2] = ins_get_vtheta();
  Eigen::Vector3d speedRerror = _speed_cons - speedR;
  Eigen::Vector3d u_speedR;
  for(int i=0; i<3; i++) {
    u_speedR[i] = speedR_pids[i].update(speedRerror[i]);
  }

  // Eigen::Vector3d  motors_pos = odometry.get_motors_pos();
  Eigen::Vector3d  motors_speed = odometry.get_motors_speed();

  Eigen::Vector3d speed_error = D*(_speed_cons + u_speedR) - motors_speed;
  
  
  // if(_pos_cascade_enabled) {
  //   Eigen::Vector3d pos_ctrl_vel = {0, 0, 0};
  //   Eigen::Vector3d pos_error = _pos_cons - motors_pos;
  //   for(int i=0; i<MOTORS_NB; i++) {
  //       pos_ctrl_vel[i] = pos_pids[i].update((double)pos_error[i]);
  //     }
  //   speed_error += pos_ctrl_vel;
  // }


  for(int i=0; i<MOTORS_NB; i++) {
    _cmds[i] = vel_pids[i].update(speed_error[i]);
  }

  mot1.set_cmd(_cmds[0]);
  mot2.set_cmd(_cmds[1]);
  mot3.set_cmd(_cmds[2]);

}
