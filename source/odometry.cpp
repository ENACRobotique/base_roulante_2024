#include "odometry.h"
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

/*
 *  |v1|   |-sin(O1)  cos(O1)  1|   |vx|
 *  |v2| = |-sin(O2)  cos(O2)  1| . |vy|
 *  |v3|   |-sin(O3)  cos(O3)  1|   |Rw|
 *
 *    m  =           D            .   v
 */

// Euclidean speeds into motor speeds: m = Dv


void Odometry::init() {
  for(size_t i=0; i<MOTORS_NB; i++) {
    motors[i].init();
  }
  
  _position = {0, 0, 0};
  prev_motors_pos = get_motors_pos();
  _speed_r = {0, 0, 0};
}


void Odometry::update() {
  // motors position in mm
  auto motors_pos = get_motors_pos();
  // motors speed in mm/s
  auto motors_speeds = get_motors_speed();
  

  //robot move in robot frame
  Eigen::Vector3d robot_move_r = Dinv * ((motors_pos - prev_motors_pos));

  //send_move(robot_move_r);

  prev_motors_pos = motors_pos;

  _speed_r = Dinv * motors_speeds;

  // hypothesis: the movement is approximated as a straight line at heading [ oldTheta + dTheta/2 ]
  double theta_mean = _position.theta() + robot_move_r[2]/2;

  // rotation matrix from robot frame to table frame
  const Eigen::Matrix<double, 3, 3> R {
    {cos(theta_mean), -sin(theta_mean), 0},
    {sin(theta_mean), cos(theta_mean) , 0},
    {0              , 0               , 1}
  };

  // // change frame from robot frame to table frame
  Position robot_move_table = Eigen::Vector3d(R * robot_move_r);

  _position += robot_move_table;
  _position.set_theta(center_radians(-ins_get_theta()));

}

#if defined(BOARD_DC)
void Odometry::update_filters()
{
  chMtxLock(&mut_hgf_pos);
  enc1.update_filter();
  enc2.update_filter();
  enc3.update_filter();
  chMtxUnlock(&mut_hgf_pos);
}
#endif

void Odometry::set_pos(float x, float y, float theta) {
  _position = {x, y, theta};
}


Eigen::Matrix<double, MOTORS_NB, 1> Odometry::get_motors_pos() {
  chMtxLock(&mut_hgf_pos);
  Eigen::Matrix<double, MOTORS_NB, 1> motors_pos =
  { motors[0].get_pos(),
    motors[1].get_pos(),
    motors[2].get_pos(),
    motors[3].get_pos()
  };
  chMtxUnlock(&mut_hgf_pos);
  return motors_pos;
}

Eigen::Matrix<double, MOTORS_NB, 1> Odometry::get_motors_speed() {
  chMtxLock(&mut_hgf_pos);
  Eigen::Matrix<double, MOTORS_NB, 1> motors_pos =
  { motors[0].get_speed(),
    motors[1].get_speed(),
    motors[2].get_speed(),
    motors[3].get_speed()
  };
  chMtxUnlock(&mut_hgf_pos);
  return motors_pos;
}


void Odometry::send_move(Eigen::Vector3d dpos) {
  e::Message<MOTORS_NB> msg;
  msg.clear();
  auto& pos = msg.mutable_pos();
  pos.set_x(dpos[0]);
  pos.set_y(dpos[1]);
  pos.set_theta(dpos[2]);
  msg.set_topic(e::Topic::MOVE_ROBOT_R);
  post_message(msg, e::Message<MOTORS_NB>::MsgType::STATUS, TIME_IMMEDIATE);
}
