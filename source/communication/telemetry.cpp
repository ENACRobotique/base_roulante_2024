#include "ch.h"
#include "hal.h"
#include "communication.h"
#include "telemetry.h"
#include "messages.h"
#include "control.h"
#include "odometry.h"
#include "globalVar.h"
#include "ins.h"

namespace e = enac;

void send_pos(e::Message<MOTORS_NB>& msg);
void send_speed(e::Message<MOTORS_NB>& msg);
void send_motor_pos(e::Message<MOTORS_NB>& msg);
void send_motor_speed(e::Message<MOTORS_NB>& msg);
void send_pos_ekf(e::Message<MOTORS_NB>& msg);

static THD_WORKING_AREA(telem, 20000);
 static void telemetry(void *) {
   chRegSetThreadName("telemetry");
   e::Message<MOTORS_NB> msg;

    while (true) {
      systime_t now = chVTGetSystemTime();
      
      send_pos(msg);
      send_speed(msg);
      send_motor_pos(msg);
      send_motor_speed(msg);
      send_ins_report(msg);
      //send_pos_ekf(msg);

      chThdSleepUntil(chTimeAddX(now,chTimeMS2I(ODOM_PERIOD_MS)));
    }
}


void telemetryStart(){

  chThdCreateStatic(telem, sizeof(telem), NORMALPRIO-1, telemetry, NULL);
}


void send_pos(e::Message<MOTORS_NB>& msg) {
  msg.clear();
  auto& pos = msg.mutable_pos();
  pos = odometry.get_pos().to_proto();
  msg.set_topic(e::Topic::POS_ROBOT_W);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}

void send_pos_ekf(e::Message<MOTORS_NB>& msg) {
  msg.clear();
  auto& pos = msg.mutable_pos();
  pos = ekf.get_pos().to_proto();
  msg.set_topic(e::Topic::POS_ROBOT_EKF);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}

void send_speed(e::Message<MOTORS_NB>& msg) {
  msg.clear();
  auto& pos = msg.mutable_speed();
  auto speed = odometry.get_speed();
  //auto vtheta = ins_get_vtheta();
  pos.set_vx(speed.vx());
  pos.set_vy(speed.vy());
  pos.set_vtheta(speed.vtheta());


  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}

void send_motor_pos([[maybe_unused]] e::Message<MOTORS_NB>& msg) {
  // msg.clear();
  // auto& motors = msg.mutable_motors();
  // auto motor_pos = odometry.get_motors_pos();
  // for(size_t i=0; i<MOTORS_NB; i++) {
  //   motors.add_m(motor_pos[i]);
  // }
  // motors.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_POS);
  // post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);

  // msg.clear();
  
  // motors = msg.mutable_motors();
  // auto motor_pos_cons = control.get_motors_pos_cons();
  // for(size_t i=0; i<MOTORS_NB; i++) {
  //   motors.add_m(motor_pos_cons[i]);
  // }
  // motors.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_POS_CONS);
  // post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);

  // msg.clear();
  
  // motors = msg.mutable_motors();
  // auto motor_cmds = control.get_motors_cmds();
  // for(size_t i=0; i<MOTORS_NB; i++) {
  //   motors.add_m(motor_cmds[i]);
  // }
  // motors.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_CMD);
  // post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);

}


void send_motor_speed(e::Message<MOTORS_NB>& msg) {
  odometry.send_motor_speed(msg);
  control.send_motor_speedcmd(msg);
  control.send_motor_speedcons(msg);
}