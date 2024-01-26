#include "holocontrol.h"
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


/*
 *  |v1|   |-sin(O1)  cos(O1)  R|   |vx|
 *  |v2| = |-sin(O2)  cos(O2)  R| . |vy|
 *  |v3|   |-sin(O3)  cos(O3)  R|   |w|
 *
 *    m  =           D            .   v
 */

// Euclidean speeds into motor speeds: m = Dv


using namespace protoduck;

void HoloControl::init() {

  motorsStart();

  set_motor(0,0);
  set_motor(1,0);
  set_motor(2,0);

  _pos_cons = {0, 0, 0};
  _speed_cons = {0, 0, 0};
  _cmds = {0, 0, 0};
  }


/**
 * pos: position setpoint in robot frame
 * speed: speed setpoint in robot frame, relative to earth
*/
void HoloControl::set_cons(Eigen::Vector3d posRobotR, Eigen::Vector3d vRobotR)
{  
    _pos_cons = (D * posRobotR) + odometry.get_motors_pos();
    _speed_cons = D * vRobotR;
}

void HoloControl::update()
{
  Eigen::Vector3d  motors_pos = odometry.get_motors_pos();
  Eigen::Vector3d  motors_speed = odometry.get_motors_speed();

  Eigen::Vector3d pos_error = _pos_cons - motors_pos;
  Eigen::Vector3d speed_error = _speed_cons - motors_speed;

  for(int i=0; i<MOTORS_NB; i++) {
    _cmds[i] = pids[i].update_cmd((double)pos_error[i], (double)speed_error[i],ODOM_PERIOD);
  }
  
    set_motor(0,_cmds[0]);
    set_motor(1,_cmds[1]);
    set_motor(2,_cmds[2]);


}