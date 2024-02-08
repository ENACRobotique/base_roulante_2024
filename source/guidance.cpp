#include "guidance.h"
#include "globalVar.h"
#include "odometry.h"
#include <math.h>
#include "messages.h"

using namespace protoduck;

constexpr double VMAX = 200; //   mm/s

void Guidance::set_target(Eigen::Vector3d pos){
    start_pos = odometry.get_pos();
    target_pos= pos;

    while (target_pos[2] - start_pos[2] > M_PI){
        target_pos[2] -= 2*M_PI;
    }
    while (target_pos[2] - start_pos[2] < -M_PI){
        target_pos[2] += 2*M_PI;
    }

    start_time = chVTGetSystemTime();
    double d = sqrt(pow(target_pos[0] - start_pos[0],2) + pow(target_pos[1]-start_pos[1],2) + 200*pow(target_pos[2] - start_pos[2],2));
    estimated_time= d/VMAX;
    state = GuidanceState::RUNNING;
    
}

void Guidance::init(){
    state = GuidanceState::IDLE;
    
};

void Guidance::update(){

    if (state == GuidanceState::RUNNING){
        if(estimated_time < 0.001) {
            state = GuidanceState::IDLE;
        }

        double dt = chTimeI2MS(chTimeDiffX(start_time,chVTGetSystemTime()))/1000.0;
        
        double alpha = dt/estimated_time;
        Eigen::Vector3d speed = {0,0,0};

        auto posRobotW = odometry.get_pos();
        double theta = posRobotW[2];
        const Eigen::Matrix<double, 3, 3> rot {
            {cos(theta) , sin(theta), 0},
            {-sin(theta), cos(theta), 0},
            {0          ,          0, 1}
        };

        if(alpha<1) {
            Eigen::Vector3d posCarrotW = alpha*target_pos + (1-alpha)*start_pos;
            Eigen::Vector3d posCarrotR = rot * (posCarrotW - posRobotW);
            
            holocontrol.set_cons(posCarrotR,speed);

            // Message msg;
            // auto& pos = msg.mutable_pos();
            // pos.set_x(posCarrotW[0]);
            // pos.set_y(posCarrotW[1]);
            // pos.set_theta(posCarrotW[2]);
            // pos.set_obj(Pos::PosObject::POS_CARROT_W);
            // post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);

        } else {
            state = GuidanceState::IDLE;
            Eigen::Vector3d posCarrotR = rot * (target_pos - posRobotW);
            holocontrol.set_cons(posCarrotR,speed);
        }
        
    }
}