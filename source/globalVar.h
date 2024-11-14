#pragma once

#include "ch.h"
#include "hal.h"
#include "holocontrol.h"
#ifdef __cplusplus
#include "encoders.h"
#include "odometry.h"
#include "guidance.h"

#define ENCODERS_PERIOD 2
#define ODOM_PERIOD 10
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


class StateHolder {
    private :
        uint32_t _asserve_state;
        uint32_t _guidance_state;
        uint32_t _odometry_state;

    public:
        uint32_t get_guidance_state(){return _guidance_state;};
        uint32_t get_control_state(){return _asserve_state;};
        uint32_t get_odom_state(){return _odometry_state;};
        void set_control(uint32_t asserve);
        void set_odometry(uint32_t guidance){_guidance_state = guidance;};
        void set_guidance(uint32_t odometry){_odometry_state = odometry};

};

StateHolder stateholder;

#endif


