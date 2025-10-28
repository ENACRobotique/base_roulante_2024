#include "guidanceDiff.h"
#include "globalVar.h"
#include "odometry.h"
#include <math.h>
#include "messages.h"
#include "mot_conf.h"
#include "Position.h" 

namespace e = enac;

constexpr double VMAX = 400; //   mm/s
constexpr double OMG_MAX = M_PI; //   rad/s

constexpr double Ke = 5; //   pente de décélaration (probablement empirique)
constexpr double Ke_ANG = M_PI;
constexpr double A = 10000; // pente d'accélération 
constexpr double A_ANG = 100*M_PI; // pente d'accélération 


constexpr double DIST_ACCURACY = 10;     //   mm
constexpr double THETA_ACCURACY = 5*DEG_TO_RAD; //   rad

void GuidanceDiff::init(){
    state = GuidanceState::IDLE;
}

void GuidanceDiff::set_target(Position pos){
    target_pos = pos;
    last_time = chVTGetSystemTime();
    
    Position posRobot = odometry.get_pos();
    Position target_pos_R = target_pos.to_frame(posRobot);
    if (abs(target_pos_R.x()) < DIST_ACCURACY) {
        state = GuidanceState::FINAL_TURN;
    } else {
        state = GuidanceState::INITIAL_TURN_TO_TARGET;
    }
}

/*
Envoi les consignes aux moteurs, step par step entre la pos initiale et la target pos
*/
void GuidanceDiff::update() {
    if (state != GuidanceState::IDLE){

        double dt = chTimeI2MS(chTimeDiffX(last_time,chVTGetSystemTime()))/1000.0;
        
        Position posRobot = odometry.get_pos();
        Speed speed = odometry.get_speed();
        Position target_pos_R = target_pos.to_frame(posRobot);

        if (state == GuidanceState::INITIAL_TURN_TO_TARGET){
            double dist_theta = target_pos_R.gisement();
            if (abs(dist_theta) < THETA_ACCURACY) {
                state = GuidanceState::AVANCING_TO_TARGET;
            }
            double speed_cons_theta;
            if (dist_theta<0){
                speed_cons_theta = max(Ke_ANG * dist_theta, max( -OMG_MAX, speed.vtheta() - A_ANG * dt));
            } 
            else {
                speed_cons_theta = min(Ke_ANG * dist_theta, min(OMG_MAX, speed.vtheta() + A_ANG * dt));
            }
            control.set_cons(Speed(0,0,speed_cons_theta));
        }


        if (state == GuidanceState::AVANCING_TO_TARGET){
            if (abs(target_pos_R.x()) < DIST_ACCURACY) {
                state = GuidanceState::FINAL_TURN;
            }
            double speed_cons_d;
            if(target_pos_R.x() < 0) {
                speed_cons_d = max(Ke * target_pos_R.x(), max(-VMAX, speed.vx() - A * dt));
            } else {
                speed_cons_d = min(Ke * target_pos_R.x(), min(VMAX, speed.vx() + A * dt));
            }
            
            double dist_theta = target_pos_R.gisement();
            double speed_cons_theta;
            if (dist_theta<0){
                speed_cons_theta = max(Ke_ANG * dist_theta, max( -OMG_MAX, speed.vtheta() - A_ANG * dt));
            } 
            else {
                speed_cons_theta = min(Ke_ANG * dist_theta, min(OMG_MAX, speed.vtheta() + A_ANG * dt));
            }

            control.set_cons(Speed(speed_cons_d,0,speed_cons_theta));
        }

        if (state == GuidanceState::FINAL_TURN){
            auto dist_theta = target_pos_R.theta();
            if (abs(dist_theta) < THETA_ACCURACY) {
                control.set_cons(Speed(0,0,0));
                state = GuidanceState::IDLE;
            }
            double speed_cons_theta;
            if (dist_theta<0){
                speed_cons_theta = max(Ke_ANG * dist_theta, max( -OMG_MAX, speed.vtheta() - A_ANG * dt));
            } 
            else {
                speed_cons_theta = min(Ke_ANG * dist_theta, min(OMG_MAX, speed.vtheta() + A_ANG * dt));
            }
            control.set_cons(Speed(0,0,speed_cons_theta));
        }
    }
    last_time = chVTGetSystemTime();
}
