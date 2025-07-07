#include "ch.h"
#include "hal.h"
#include "communication.h"
#include "telemetry.h"
#include "messages.h"
#include "odometry.h"
#include "globalVar.h"
#include "ins.h"

namespace e = enac;

void send_pos(e::Message<MOTORS_NB>& msg);
void send_speed(e::Message<MOTORS_NB>& msg);
void send_motor_pos(e::Message<MOTORS_NB>& msg);
void send_motor_speed(e::Message<MOTORS_NB>& msg);

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

     chThdSleepUntil(chTimeAddX(now,chTimeMS2I(ODOM_PERIOD)));
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

void send_speed(e::Message<MOTORS_NB>& msg) {
  msg.clear();
  auto& pos = msg.mutable_speed();
  auto speed = odometry.get_speed();
  auto vtheta = ins_get_vtheta();
  pos.set_vx(speed[0]);
  pos.set_vy(speed[1]);
  pos.set_vtheta(vtheta);

  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}

void send_motor_pos(e::Message<MOTORS_NB>& msg) {
  msg.clear();
  auto& motors = msg.mutable_motors();
  auto motor_pos = odometry.get_motors_pos();
  for(size_t i=0; i<MOTORS_NB; i++) {
    motors.add_m(motor_pos[i]);
  }
  motors.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_POS);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);

  msg.clear();
  
  motors = msg.mutable_motors();
  auto motor_pos_cons = holocontrol.get_motors_pos_cons();
  for(size_t i=0; i<MOTORS_NB; i++) {
    motors.add_m(motor_pos_cons[i]);
  }
  motors.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_POS_CONS);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);

  msg.clear();
  
  motors = msg.mutable_motors();
  auto motor_cmds = holocontrol.get_motors_cmds();
  for(size_t i=0; i<MOTORS_NB; i++) {
    motors.add_m(motor_cmds[i]);
  }
  motors.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_CMD);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);

}


void send_motor_speed(e::Message<MOTORS_NB>& msg) {
  msg.clear();
  auto& motors = msg.mutable_motors();
  auto motor_speed = odometry.get_motors_speed();
  for(size_t i=0; i<MOTORS_NB; i++) {
    motors.add_m(motor_speed[i]);
  }
  motors.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_SPEED);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);

  msg.clear();
  
  motors = msg.mutable_motors();
  auto motor_speed_cons = holocontrol.get_motors_speed_cons();
  for(size_t i=0; i<MOTORS_NB; i++) {
    motors.add_m(motor_speed_cons[i]);
  }
  motors.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_SPEED_CONS);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);

  msg.clear();
  
  motors = msg.mutable_motors();
  auto motor_cmds = holocontrol.get_motors_cmds();
  for(size_t i=0; i<MOTORS_NB; i++) {
    motors.add_m(motor_cmds[i]);
  }
  motors.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_CMD);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);

}