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

constexpr double Ke = 8; //   pente de décélaration (probablement empirique)
constexpr double Ke_ANG = M_PI*2;
constexpr double A = 2000; // pente d'accélération 
constexpr double A_ANG = 10*M_PI; // pente d'accélération angulaire

//constexpr double look_ahead_d = 10;

constexpr double DIST_ACCURACY = 5;     //   mm
constexpr double THETA_ACCURACY = 1*DEG_TO_RAD; //   rad

void GuidanceDiff::init(){
    state = GuidanceState::IDLE;
}

void GuidanceDiff::set_target(Position pos, std::optional<double> direction) {
    Position posRobot = odometry.get_pos();
    double dir;
    if(direction.has_value()) {
        dir = direction.value();
    } else {
        
        Position target_pos_R = pos.to_frame(posRobot);
        dir = target_pos_R.gisement();
    }
    if (abs(dir) < M_PI/2) mvt_direction = MvtDirection::FORWARD;
    else mvt_direction = MvtDirection::BACKWARD;

    target_pos = pos;
    last_time = chVTGetSystemTime();

    double dist = posRobot.distance(pos);
    if (dist < DIST_ACCURACY) {
        state = GuidanceState::FINAL_TURN;
    } else {
        state = GuidanceState::INITIAL_TURN_TO_TARGET;
        //state = GuidanceState::COURBE;
    }


    if(dist > 400) {
        vmax = VMAX ;
        omg_max = OMG_MAX;
        ke = Ke;
        ke_ang = Ke_ANG;
        a = A;
        a_ang = A_ANG;
    } else {
        vmax = VMAX / 1.5;
        omg_max = OMG_MAX/1.5;
        ke = Ke / 1.5;
        ke_ang = Ke_ANG;
        a = A / 1.5;
        a_ang = A_ANG;
    }
}

// void GuidanceDiff::set_trajectoire(Position* traj){}

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

            // Si on y va en arriere
            if (mvt_direction == MvtDirection::BACKWARD){
                if (dist_theta>0) dist_theta = dist_theta - M_PI ;
                else dist_theta = M_PI + dist_theta;
            }


            if (abs(dist_theta) < THETA_ACCURACY) {
                state = GuidanceState::AVANCING_TO_TARGET;
            }

            double speed_cons_theta;

            if (dist_theta<0){
                speed_cons_theta = max(ke_ang * dist_theta, max( -omg_max, speed.vtheta() - a_ang * dt));
            } 
            else {
                speed_cons_theta = min(ke_ang * dist_theta, min(omg_max, speed.vtheta() + a_ang * dt));
            }
            control.set_cons(Speed(0,0,speed_cons_theta));
        }
    
        if (state == GuidanceState::AVANCING_TO_TARGET){
            if (abs(target_pos_R.x()) < DIST_ACCURACY) {
                state = GuidanceState::FINAL_TURN;
            }
            double speed_cons_d;
            if(target_pos_R.x() < 0) {
                speed_cons_d = max(ke * target_pos_R.x(), max(-vmax, speed.vx() - a * dt));
            } else {
                speed_cons_d = min(ke * target_pos_R.x(), min(vmax, speed.vx() + a * dt));
            }
            
            double dist_theta = target_pos_R.gisement();

            if (mvt_direction == MvtDirection::BACKWARD){
                if (dist_theta>0) dist_theta = dist_theta - M_PI ;
                else dist_theta = M_PI + dist_theta;
            }

            double speed_cons_theta;
            if (dist_theta<0){
                speed_cons_theta = max(ke_ang * dist_theta, max( -omg_max, speed.vtheta() - a_ang * dt));
            } 
            else {
                speed_cons_theta = min(ke_ang * dist_theta, min(omg_max, speed.vtheta() + a_ang * dt));
            }

            control.set_cons(Speed( speed_cons_d,0, speed_cons_theta));
        }

        

        if (state == GuidanceState::COURBE) {
            if (abs(target_pos.distance(posRobot)) < DIST_ACCURACY*5) {
                state = GuidanceState::FINAL_TURN;
            }

            double dist_theta = target_pos_R.gisement();
            double alpha_courbe = sqrt(pow(target_pos_R.y(),2))/sqrt(pow(target_pos_R.x(),2) +  pow(target_pos_R.y(),2)) ;

            if (abs(dist_theta) < M_PI/2) mvt_direction = MvtDirection::FORWARD;
            else mvt_direction = MvtDirection::BACKWARD;
            
            // Si on y va en arriére
            if (mvt_direction == MvtDirection::BACKWARD){
                if (dist_theta>0) dist_theta = dist_theta - M_PI ;
                else dist_theta = M_PI + dist_theta;
            }

            double speed_cons_theta;
            if (dist_theta<0){
                speed_cons_theta = max(Ke_ANG * dist_theta, max( -OMG_MAX, speed.vtheta() - A_ANG * dt)); //+ alpha * speed_cons_theta_f;
            } 
            else {
                speed_cons_theta = min(Ke_ANG * dist_theta, min(OMG_MAX, speed.vtheta() + A_ANG * dt)); // + alpha * speed_cons_theta_f;
            }

            if (sqrt(pow(target_pos_R.x(),2) +  pow(target_pos_R.y(),2)) < DIST_ACCURACY) {
                state = GuidanceState::FINAL_TURN;
            }

            double speed_cons_d;
            if(target_pos_R.x() < 0) {
                speed_cons_d = max(- Ke * sqrt(pow(target_pos_R.x(),2) +  pow(target_pos_R.y(),2)), max(-VMAX, speed.vx() - A * dt));
            } else {
                speed_cons_d = min(Ke * sqrt(pow(target_pos_R.x(),2) +  pow(target_pos_R.y(),2)), min(VMAX, speed.vx() + A * dt));
            }

            control.set_cons(Speed((1-alpha_courbe) * speed_cons_d, 0, alpha_courbe * speed_cons_theta));
        }

        if (state == GuidanceState::FINAL_TURN){
            auto dist_theta = target_pos_R.theta();
            if (abs(dist_theta) < THETA_ACCURACY) {
                control.set_cons(Speed(0,0,0));
                state = GuidanceState::IDLE;
            }
            double speed_cons_theta;
            if (dist_theta<0){
                speed_cons_theta = max(ke_ang * dist_theta, max( -omg_max, speed.vtheta() - a_ang * dt));
            } 
            else {
                speed_cons_theta = min(ke_ang * dist_theta, min(omg_max, speed.vtheta() + a_ang * dt));
            }
            control.set_cons(Speed(0,0,speed_cons_theta));
        }
    
        // if (state == GuidanceState::PURE_PURSUIT){
        // }
    }
    last_time = chVTGetSystemTime();
}
