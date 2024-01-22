#include "utils.h"
#include "BytesWriteBuffer.h"
#include "ch.h"
#include "communication.h"


using namespace protoduck;

/**
 * Centers an angle in radians to [-pi, pi[
 */
double center_radians(double angle){
  while (angle >= M_PI){
    angle -= 2 * M_PI;
  }
  while (angle < -M_PI){
    angle += 2 * M_PI;
  }
  return angle;
}


double clamp(double lo, double val, double hi) {
    if(val < lo) {return lo;}
    if(val > hi) {return hi;}
    return val;
}


msg_t msg_send_pos(Eigen::Vector3d pos, Pos::PosObject obj) {
  Message msg;
  auto& pos_report = msg.mutable_pos();
  pos_report.set_obj(obj);
  pos_report.set_x(pos[0]);
  pos_report.set_y(pos[1]);
  pos_report.set_theta(pos[2]);
  return post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);
}

msg_t msg_send_motors(Eigen::Vector3d mot, Motors::MotorDataType obj) {
  Message msg;
  auto& pos_report = msg.mutable_motors();
  pos_report.set_type(obj);
  pos_report.set_m1(mot[0]);
  pos_report.set_m2(mot[1]);
  pos_report.set_m3(mot[2]);
  return post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);
}
