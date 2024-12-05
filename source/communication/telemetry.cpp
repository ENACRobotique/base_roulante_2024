#include "ch.h"
#include "hal.h"
#include "communication.h"
#include "telemetry.h"
#include "messages.h"
#include "odometry.h"
#include "globalVar.h"
#include "ins.h"

using namespace protoduck;

void send_pos(Message& msg);
void send_speed(Message& msg);
void send_motor_pos(Message& msg);
void send_motor_speed(Message& msg);

static THD_WORKING_AREA(telem, 20000);
 static void telemetry(void *) {
   chRegSetThreadName("telemetry");

   while (true) {
     Message msg;
     send_pos(msg);
     send_speed(msg);
     send_motor_pos(msg);
     send_motor_speed(msg);
     //send_ins_report(msg);

     
     chThdSleepMilliseconds(100);
   }
 }


void telemetryStart(){

  chThdCreateStatic(telem, sizeof(telem), NORMALPRIO-1, telemetry, NULL);
}


void send_pos(Message& msg) {
  msg.clear();
  auto& pos = msg.mutable_pos();
  pos.set_x(odometry.get_x());
  pos.set_y(odometry.get_y());
  pos.set_theta(odometry.get_theta());
  pos.set_obj(Pos::PosObject::POS_ROBOT_W);

  post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);
}

void send_speed(Message& msg) {
  msg.clear();
  auto& pos = msg.mutable_speed();
  auto speed = odometry.get_speed();
  pos.set_vx(speed[0]);
  pos.set_vy(speed[1]);
  pos.set_vtheta(speed[2]);

  post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);
}

void send_motor_pos(Message& msg) {
  msg.clear();
  auto& motors = msg.mutable_motors();
  Eigen::Vector3d motor_pos = odometry.get_motors_pos();
  motors.set_m1(motor_pos[0]);
  motors.set_m2(motor_pos[1]);
  motors.set_m3(motor_pos[2]);
  motors.set_type(Motors::MotorDataType::MOTORS_POS);
  post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);

  msg.clear();
  
  motors = msg.mutable_motors();
  Eigen::Vector3d motor_pos_cons = holocontrol.get_pos_cons();
  motors.set_m1(motor_pos_cons[0]);
  motors.set_m2(motor_pos_cons[1]);
  motors.set_m3(motor_pos_cons[2]);
  motors.set_type(Motors::MotorDataType::MOTORS_POS_CONS);
  post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);

  msg.clear();
  
  motors = msg.mutable_motors();
  Eigen::Vector3d motor_cmds = holocontrol.get_cmds();
  motors.set_m1(motor_cmds[0]);
  motors.set_m2(motor_cmds[1]);
  motors.set_m3(motor_cmds[2]);
  motors.set_type(Motors::MotorDataType::MOTORS_CMD);
  post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);

}


void send_motor_speed(Message& msg) {
  msg.clear();
  auto& motors = msg.mutable_motors();
  Eigen::Vector3d motor_speed = odometry.get_motors_speed();
  motors.set_m1(motor_speed[0]);
  motors.set_m2(motor_speed[1]);
  motors.set_m3(motor_speed[2]);
  motors.set_type(Motors::MotorDataType::MOTORS_SPEED);
  post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);

  msg.clear();
  
  motors = msg.mutable_motors();
  Eigen::Vector3d motor_speed_cons = holocontrol.get_speed_cons();
  motors.set_m1(motor_speed_cons[0]);
  motors.set_m2(motor_speed_cons[1]);
  motors.set_m3(motor_speed_cons[2]);
  motors.set_type(Motors::MotorDataType::MOTORS_SPEED_CONS);
  post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);

  msg.clear();
  
  motors = msg.mutable_motors();
  Eigen::Vector3d motor_cmds = holocontrol.get_cmds();
  motors.set_m1(motor_cmds[0]);
  motors.set_m2(motor_cmds[1]);
  motors.set_m3(motor_cmds[2]);
  motors.set_type(Motors::MotorDataType::MOTORS_CMD);
  post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);

}