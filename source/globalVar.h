#pragma once

#include "ch.h"
#include "hal.h"
#include "holocontrol.h"
#ifdef __cplusplus
#include "encoders.h"
#include "odometry.h"
#include "guidance.h"

#define ENCODERS_PERIOD 2
#define ODOM_PERIOD 20 //10
//Nom pour les commandes ( plus lisible que 1,2,...)
#define X 0
#define Y 1
#define THETA 2

extern Encoder enc1;
extern Encoder enc2;
extern Encoder enc3;

extern Odometry odometry;
extern HoloControl holocontrol;
extern Guidance guidance;


class SystemManager {
    private :
        uint32_t _asserve_state;
        uint32_t _guidance_state;
        uint32_t _odometry_state;

    public:
        uint32_t get_guidance_state(){return _guidance_state;};
        uint32_t get_control_state(){return _asserve_state;};
        uint32_t get_odom_state(){return _odometry_state;};
        void init(){
            set_odometry((uint32_t)protoduck::System::OdometryFlags::ODOMETRY_ENABLED);
            set_guidance((uint32_t)protoduck::System::GuidanceFlags::GUIDANCE_BASIC);
            set_control((uint32_t)protoduck::System::AsservFlags::ASSERV_SPEED);
        }
        void set_control(uint32_t asserve){
            _asserve_state = asserve;
            if (asserve & (uint32_t)protoduck::System::AsservFlags::ASSERV_SPEED){
                holocontrol.enable_position_control(false);}

            if (asserve & (uint32_t)protoduck::System::AsservFlags::ASSERV_POS){
                holocontrol.enable_position_control(true);}
            }

        void set_odometry(uint32_t guidance){_guidance_state = guidance;};
        void set_guidance(uint32_t odometry){_odometry_state = odometry;};

};



#endif


