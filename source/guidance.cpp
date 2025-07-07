#include "guidance.h"
#include "globalVar.h"
#include "odometry.h"
#include <math.h>
#include "messages.h"
#include "mot_conf.h"
 
namespace e = enac;

constexpr double VMAX = 200; //   mm/s

constexpr double XY_ACCURACY = 5;     //   mm
constexpr double THETA_ACCURACY = 1*DEG_TO_RAD; //   rad

void Guidance::init(){
    state = GuidanceState::IDLE;
}

void Guidance::set_target(Eigen::Vector3d pos, bool dumb){
    start_pos = odometry.get_pos().to_eigen();
    target_pos = pos;
    start_time = chVTGetSystemTime();

    Eigen::Vector3d diff = target_pos - start_pos;
    if(!dumb){
        diff[THETA] = center_radians(diff[THETA]);
    }

    double d = sqrt(pow(diff[X], 2) + pow(diff[Y], 2) + pow(ROBOT_RADIUS*diff[THETA], 2));
    estimated_time= d / VMAX; // calcule le temps estimé pour aller de start à target
    state = GuidanceState::RUNNING;


    double theta0 = start_pos[THETA];
    double thetaTarget = target_pos[THETA];

    if(!dumb){
        if(thetaTarget - theta0 > M_PI) {
            // ex: thetaTarget = pi - 0.1, theta0 = -pi + 01 ==> thetaTarget= -pi - 0.1
            // donc thetaTarget -= 2*pi
            thetaTarget -= 2*M_PI;
        } else if(thetaTarget - theta0 < -M_PI) {
            // c'est l'opposé
            thetaTarget += 2*M_PI;
        }
        target_pos[THETA] = thetaTarget;
    }
}

/*
Envoi les consignes aux moteurs, step par step entre la pos initiale et la target pos
*/
void Guidance::update() {
    if (state == GuidanceState::RUNNING){
        if(estimated_time < 0.001) {
            state = GuidanceState::IDLE;
        }

        double dt = chTimeI2MS(chTimeDiffX(start_time,chVTGetSystemTime()))/1000.0;
        
        double alpha = dt/estimated_time;
        Eigen::Vector3d speed = {0,0,0};

        auto posRobotW = odometry.get_pos();
        double theta = posRobotW.theta();
        const Eigen::Matrix<double, 3, 3> rot {
            {cos(theta) , sin(theta), 0},
            {-sin(theta), cos(theta), 0},
            {0          ,          0, 1}
        };

        Eigen::Vector3d posCarrotW;
        if(alpha < 1) {
            posCarrotW = alpha*target_pos + (1-alpha)*start_pos;
        } else {
            posCarrotW = target_pos;
        }

        Eigen::Vector3d posCarrotR = rot * (posCarrotW - posRobotW.to_eigen());

        posCarrotR[THETA] = center_radians(posCarrotR[THETA]);

        // sur le trajet, soit moins regardant sur le theta, à la fin, soit plus sévère.
        double coef_theta = min(0.6+exp((alpha-1.0)/0.1), 4.0);
        posCarrotR[THETA] = posCarrotR[THETA] * coef_theta;

        
        holocontrol.set_cons(posCarrotR,speed); //envoi le set cons en robotFrame


        if(alpha >= 1) {
            double d = sqrt(pow(posCarrotR[X], 2) + pow(posCarrotR[Y], 2));
            if(d < XY_ACCURACY && posCarrotR[THETA] < THETA_ACCURACY) {
                state = GuidanceState::IDLE;
            }
        }

        // Message msg;
        // auto& pos = msg.mutable_pos();
        // pos.set_x(posCarrotW[0]);
        // pos.set_y(posCarrotW[1]);
        // pos.set_theta(posCarrotW[2]);
        // msg.set_topic(e::Topic::POS_CARROT_W);
        // post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);
    }
}
