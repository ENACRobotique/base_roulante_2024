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

/*
 *  |v1|   |-sin(O1)  cos(O1)  1|   |vx|
 *  |v2| = |-sin(O2)  cos(O2)  1| . |vy|
 *  |v3|   |-sin(O3)  cos(O3)  1|   |Rw|
 *
 *    m  =           D            .   v
 */

// Euclidean speeds into motor speeds: m = Dv


constexpr double THETA1 = 0;
constexpr double THETA2 = -2.0*M_PI/3.0;
constexpr double THETA3 = 2.0*M_PI/3.0;

// Euclidean speeds into motor speeds: m = Dv
const Eigen::Matrix<double, 3, 3> D {
  {-sin(THETA1), cos(THETA1), ROBOT_RADIUS},
  {-sin(THETA2), cos(THETA2), ROBOT_RADIUS},
  {-sin(THETA3), cos(THETA3), ROBOT_RADIUS}
};

//motor speeds into Euclidean speeds: v = Dinv m
const Eigen::Matrix<double, 3, 3> Dinv = D.inverse();


void Odometry::init() {
  enc1.init(false);
  enc2.init(false);
  enc3.init(false);
  
  _position = {0, 0, 0};
  prev_motors_pos = get_motors_pos();
  _speed_r = {0, 0, 0};
}


void Odometry::update() {
  // motors position in mm
  Eigen::Vector3d motors_pos = get_motors_pos();
  // motors speed in mm/s
  Eigen::Vector3d motors_speeds = get_motors_speed();
  

  // robot move in robot frame
  Eigen::Vector3d robot_move_r = Dinv * ((motors_pos - prev_motors_pos));

  prev_motors_pos = motors_pos;

  _speed_r = Dinv * motors_speeds;

  // hypothesis: the movement is approximated as a straight line at heading [ oldTheta + dTheta/2 ]
  double theta_mean = _position[2] + robot_move_r[2]/2;

  // rotation matrix from robot frame to table frame
  const Eigen::Matrix<double, 3, 3> R {
    {cos(theta_mean), -sin(theta_mean), 0},
    {sin(theta_mean), cos(theta_mean) , 0},
    {0              , 0               , 1}
  };

  // change frame from robot frame to table frame
  Eigen::Vector3d robot_move_table = R * robot_move_r;

  _position += robot_move_table;
  _position(2) = center_radians(_position(2));
}


void Odometry::update_filters()
{
  chMtxLock(&mut_hgf_pos);
  enc1.update_filter();
  enc2.update_filter();
  enc3.update_filter();
  chMtxUnlock(&mut_hgf_pos);
}


void Odometry::set_pos(double x, double y, double theta) {
  _position = {x, y, theta};
}


Eigen::Vector3d Odometry::get_motors_pos() {
  chMtxLock(&mut_hgf_pos);
  Eigen::Vector3d motors_pos =
  { enc1.get_pos(),
    enc2.get_pos(),
    enc3.get_pos()};
  chMtxUnlock(&mut_hgf_pos);
  return motors_pos;
}

Eigen::Vector3d Odometry::get_motors_speed() {
  chMtxLock(&mut_hgf_pos);
  Eigen::Vector3d motors_pos =
  { enc1.get_speed(),
    enc2.get_speed(),
    enc3.get_speed()};
  chMtxUnlock(&mut_hgf_pos);
  return motors_pos;
}
