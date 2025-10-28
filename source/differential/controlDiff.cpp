#include "controlDiff.h"
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
#include "mot_conf.h"


namespace e = enac;

void ControlDiff::init() {

    motorsStart();
    stop_motors();

    robot_vit_d_cons = 0;
    robot_vit_ang_cons = 0;
    d_pid.init(1000.0/ODOM_PERIOD_MS, 100);
    ang_pid.init(1000.0/ODOM_PERIOD_MS, 100);
  

    _last_setpoint = chSysGetRealtimeCounterX();

    d_pid.set_gains(0.1, 0, 0);
    ang_pid.set_gains(0.1, 0, 0);

}


/**
 * pos: position setpoint in robot frame
 * speed: speed setpoint in robot frame, relative to earth
*/
void ControlDiff::set_cons(Speed vRobotR)
{  
    robot_vit_d_cons = vRobotR.vx();
    robot_vit_ang_cons = vRobotR.vtheta();
    _last_setpoint = chVTGetSystemTime();
}


void ControlDiff::set_vit_d_pid_gains(double kp, double ki, double kd){
    d_pid.set_gains(kp, ki, kd);
}

void ControlDiff::set_vit_ang_pid_gains(double kp, double ki, double kd){
    ang_pid.set_gains(kp, ki, kd);
}



void ControlDiff::update()
{
  if (chTimeI2MS(chVTTimeElapsedSinceX(_last_setpoint)) > 500) {
    robot_vit_d_cons = 0;
    robot_vit_ang_cons = 0;
    stop_motors();
  }
  else {
  auto speed = odometry.get_speed();

  double error_vit_d = speed.vx() - robot_vit_d_cons;
  double error_vit_ang = speed.vtheta() - robot_vit_ang_cons;

  cmd_d = d_pid.update(error_vit_d);
  cmd_ang = ang_pid.update(error_vit_ang);
  double mot_gauche_vit_cmd = cmd_d - (ROBOT_RADIUS * cmd_ang);
  double mot_droit_vit_cmd  = cmd_d + (ROBOT_RADIUS * cmd_ang);

  motors[0].set_cmd(mot_gauche_vit_cmd);
  motors[1].set_cmd(-mot_droit_vit_cmd);
  }
}

void ControlDiff::stop_motors()
{
    for (size_t i = 0; i < MOTORS_NB; i++)
    {
        motors[i].set_cmd(0);
    }
}

void ControlDiff::send_motor_speedcmd(e::Message<MOTORS_NB>& msg) {
  msg.clear();
  
  auto& motors_msg = msg.mutable_motors();

  motors_msg.add_m(cmd_d);
  motors_msg.add_m(cmd_ang);


  motors_msg.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_CMD);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}

void ControlDiff::send_motor_speedcons(e::Message<MOTORS_NB>& msg) {
  msg.clear();
  
  auto& motors_msg = msg.mutable_motors();

  motors_msg.add_m(robot_vit_d_cons);
  motors_msg.add_m(robot_vit_ang_cons);

  motors_msg.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_SPEED_CONS);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);

}