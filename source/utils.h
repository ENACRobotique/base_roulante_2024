
#pragma once

#include "messages.h"
#include "ch.h"
#include <Eigen/LU>
#include "mot_conf.h"

namespace e = enac;
#define M_PI 3.14159265358979323846

constexpr double DEG_TO_RAD = M_PI / 180.0;

#ifndef ABS
#define ABS(val) ((val) < 0 ? -(val) : (val))
#endif

template<typename T>
T min(T a, T b) {
    return a > b ? b: a;
}

/**
 * Centers an angle in radians to [-pi, pi[
 */
double center_radians(double angle);

double clamp(double lo, double val, double hi);

msg_t post_message(e::Message<MOTORS_NB>& msg, e::Message<MOTORS_NB>::MsgType msg_type, sysinterval_t timeout);

msg_t msg_send_pos(Eigen::Vector3d pos, e::Topic topic);
msg_t msg_send_motors(Eigen::Matrix<double, MOTORS_NB, 1> mot, e::Motors<MOTORS_NB>::MotorDataType obj);
