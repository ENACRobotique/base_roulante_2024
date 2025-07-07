#include "utils.h"
#include "BytesWriteBuffer.h"
#include "ch.h"
#include "communication.h"

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


msg_t msg_send_pos(Eigen::Vector3d pos, e::Topic topic) {
  e::Message<MOTORS_NB> msg;
  auto& pos_report = msg.mutable_pos();
  msg.set_topic(topic);
  pos_report.set_x(pos[0]);
  pos_report.set_y(pos[1]);
  pos_report.set_theta(pos[2]);
  return post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}

msg_t msg_send_motors(Eigen::Matrix<double, MOTORS_NB, 1> mot, e::Motors<MOTORS_NB>::MotorDataType obj) {
  e::Message<MOTORS_NB> msg;
  auto& pos_report = msg.mutable_motors();
  pos_report.set_type(obj);
  for(size_t i=0; i<MOTORS_NB; i++) {
    pos_report.add_m(mot[i]);
  }
  return post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}



void std::__throw_out_of_range_fmt(char const*, ...) {
  chSysHalt("out of range");
  while(true);
}
