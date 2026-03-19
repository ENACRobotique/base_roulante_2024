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

// constexpr double VMAX = 400; //   mm/s
// constexpr double OMG_MAX = M_PI; //   rad/s

// constexpr double K = 70;//77;
// constexpr double tau = 0.1*8*0;

void ControlDiff::init() {

    motorsStart();
    stop_motors();

    target_speed = Speed(0,0,0);
    d_pid.init(1000.0/ODOM_PERIOD_MS, 100);
    ang_pid.init(1000.0/ODOM_PERIOD_MS, 100);
  

    _last_setpoint = chSysGetRealtimeCounterX();

    d_pid.set_gains(0.15, 0.3, 0.015);
    ang_pid.set_gains(0.15, 0.2, 0.022);

}

/**
 * pos: position setpoint in robot frame
 * speed: speed setpoint in robot frame, relative to earth
*/
void ControlDiff::set_cons(Speed vRobotR)
{  
    target_speed = vRobotR;
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
  double dt = chTimeI2MS(chTimeDiffX(last_time,chVTGetSystemTime()))/1000.0;
  if (chTimeI2MS(chVTTimeElapsedSinceX(_last_setpoint)) > 500) {
    target_speed = Speed(0,0,0);
    cons_speed = Speed(0,0,0);
    stop_motors();
  }
  else {
  auto speed = odometry.get_speed();

  double Vmin = -A_max*dt + cons_speed.vx();
  double Vmax = A_max*dt + cons_speed.vx();
  double Vmin_ang = -A_ang_max*dt + cons_speed.vtheta();
  double Vmax_ang = A_ang_max*dt + cons_speed.vtheta();


  double clamps = clamp(Vmin,target_speed.vx(),Vmax);
  double clamps_ang = clamp(Vmin_ang,target_speed.vtheta(),Vmax_ang);
  cons_speed = Speed(clamps,0,clamps_ang);

  float error_vit_d = speed.vx() - cons_speed.vx();
  float error_vit_ang = speed.vtheta() - cons_speed.vtheta();

  // cmd_d = cmd_d_feedforward;
  cmd_d = d_pid.update(error_vit_d);
  cmd_ang = ang_pid.update(error_vit_ang);
  
  // test open loop
  // cmd_d = -cons_speed.vx() * 0.05;
  // cmd_ang = -cons_speed.vtheta() * 0.05;
  ////////////////////////

  double mot_gauche_vit_cmd = cmd_d - (ROBOT_RADIUS * cmd_ang);
  double mot_droit_vit_cmd  = cmd_d + (ROBOT_RADIUS * cmd_ang);

  motors[0].set_cmd(mot_gauche_vit_cmd);
  motors[1].set_cmd(-mot_droit_vit_cmd);
  }
  last_time = chVTGetSystemTime();
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

  motors_msg.add_m(cons_speed.vx());
  motors_msg.add_m(cons_speed.vtheta());

  motors_msg.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_SPEED_CONS);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);

}