#include "odometryDiff.h"
#include "encoders.h"
#include "utils.h"
#include "communication.h"
#include "BytesWriteBuffer.h"
#include "messages.h"
#include <cmath>
#include <Eigen/LU>

#include "globalVar.h"
#include "stdutil.h"
#include "printf.h"
#include "ins.h"
#include "mot_conf.h"

#if DRIVE == DRIVE_DIFF

void OdometryDiff::init() {
  motors[0].init();
  motors[1].init();
  _position = {0, 0, 0};
  mot_gauche_prev_pos = motors[0].get_pos();
  mot_droit_prev_pos = motors[1].get_pos();
  // _speed_r = {0, 0, 0};
}


void OdometryDiff::update() {
  double mot_gauche_pos = -motors[0].get_pos();
  double mot_droit_pos = motors[1].get_pos();
  double d_dist = ((mot_gauche_pos - mot_gauche_prev_pos) + (mot_droit_pos - mot_droit_prev_pos))/2;
  double d_theta = ((mot_droit_pos - mot_droit_prev_pos) - (mot_gauche_pos - mot_gauche_prev_pos))/(ROBOT_RADIUS*2);

  _position += Position(d_dist * cos(_position.theta() + d_theta/2), d_dist * sin(_position.theta() + d_theta/2),d_theta);

  // Attention
  _position.set_theta(ins_get_theta());

  _position.center_theta();
  // send_move(robot_move_r);
  double mot_gauche_vit = -motors[0].get_speed();
  double mot_droit_vit = motors[1].get_speed();

  robot_vit_d = (mot_gauche_vit + mot_droit_vit)/2;
  robot_vit_ang = (mot_droit_vit - mot_gauche_vit)/(ROBOT_RADIUS*2);

  _speed_r = Speed(robot_vit_d,0,robot_vit_ang);

  //Speed _speed_table = Speed(robot_vit_d * cos(_position.theta()),robot_vit_d * sin(_position.theta()),robot_vit_ang);

  mot_gauche_prev_pos = mot_gauche_pos;
  mot_droit_prev_pos = mot_droit_pos;

}
  
void OdometryDiff::set_pos(Position pos) {
  _position = pos;
}

void OdometryDiff::send_move(Position dpos) {
  e::Message<MOTORS_NB> msg;
  msg.clear();
  auto& pos = msg.mutable_pos();
  pos.set_x(dpos.x());
  pos.set_y(dpos.y());
  pos.set_theta(dpos.theta());
  msg.set_topic(e::Topic::MOVE_ROBOT_R);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}

void OdometryDiff::send_motor_speed(e::Message<MOTORS_NB>& msg) {
  msg.clear();
  auto& motors_msg = msg.mutable_motors();
  for(size_t i=0; i<MOTORS_NB; i++) {
    motors_msg.add_m(motors[i].get_speed());
  }
  motors_msg.set_type(e::Motors<MOTORS_NB>::MotorDataType::MOTORS_SPEED);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}

#endif