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

  set_motor(0,0);
  set_motor(1,0);
  set_motor(2,0);

  _pos_cons = {0., 0., 0.};
  _speed_cons = {0., 0., 0.};
  _cmds = {0., 0., 0.};

  last_setpoint = chSysGetRealtimeCounterX();

  for(int i = 0; i<3;i++){
    vel_pids[i].init(ODOM_PERIOD, 10);
    vel_pids[i].set_gains(0.5, 0.1, 0);

    pos_pids[i].init(ODOM_PERIOD, 10);
    pos_pids[i].set_gains(2, 0.1, 1);
  }
  
  pos_cascade_enabled = false;


}


/**
 * pos: position setpoint in robot frame
 * speed: speed setpoint in robot frame, relative to earth
*/
void HoloControl::set_cons(const Eigen::Vector3d& posRobotR, const Eigen::Vector3d& vRobotR)
{  
    _pos_cons = (D * posRobotR) + odometry.get_motors_pos();
    _speed_cons = D * vRobotR;
    last_setpoint = chVTGetSystemTime();
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
  Eigen::Vector3d  motors_pos = odometry.get_motors_pos();
  Eigen::Vector3d  motors_speed = odometry.get_motors_speed();

  Eigen::Vector3d pos_error = _pos_cons - motors_pos;

  Eigen::Vector3d pos_ctrl_vel = {0, 0, 0};

  Eigen::Vector3d speed_error = _speed_cons - motors_speed;
  
  if(pos_cascade_enabled) {
    for(int i=0; i<MOTORS_NB; i++) {
        pos_ctrl_vel[i] = pos_pids[i].update((double)pos_error[i]);
      }
    speed_error = _speed_cons + pos_ctrl_vel - motors_speed;
  }


  for(int i=0; i<MOTORS_NB; i++) {
    _cmds[i] = vel_pids[i].update(speed_error[i]);
  }
  
  set_motor(0,_cmds[0]);
  set_motor(1,_cmds[1]);
  set_motor(2,_cmds[2]);

  //DebugTrace("v: %f %f %f", _cmds[0], _cmds[1], _cmds[2]);


    if (chTimeI2MS(chVTTimeElapsedSinceX(last_setpoint)) > 1000) {
      _speed_cons = {0,0,0};
    }


}
