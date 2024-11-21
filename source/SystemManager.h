#pragma once
#include "holocontrol.h"
#include "messages.h"


class SystemManager {
    private:
        uint32_t _asserve_state;
        uint32_t _guidance_state;
        uint32_t _odometry_state;
        HoloControl * _holocontrol;


    public:
        uint32_t get_guidance_state(){return _guidance_state;};
        uint32_t get_control_state(){return _asserve_state;};
        uint32_t get_odom_state(){return _odometry_state;};

        /*Default values :
        - ODOMETRY_ENABLED
        - GUIDANCE_BASIC
        - ASSERV_SPEED
        */
        void init(HoloControl * holo){
            _holocontrol = holo;
            set_odometry((uint32_t)protoduck::System::OdometryFlags::ODOMETRY_ENABLED);
            set_guidance((uint32_t)protoduck::System::GuidanceFlags::GUIDANCE_BASIC);
            set_control((uint32_t)protoduck::System::AsservFlags::ASSERV_POS);
        }
        void set_control(uint32_t asserve){
            _asserve_state = asserve;
            if (asserve & (uint32_t)protoduck::System::AsservFlags::ASSERV_SPEED){
                _holocontrol->enable_position_control(false);}

            if (asserve & (uint32_t)protoduck::System::AsservFlags::ASSERV_POS){
                _holocontrol->enable_position_control(true);}
            }

        void set_odometry(uint32_t guidance){_guidance_state = guidance;};
        void set_guidance(uint32_t odometry){_odometry_state = odometry;};

};