#pragma once
#include "holocontrol.h"
#include "odometry.h"
#include "messages.h"
#include "guidance.h"


class SystemManager {
    private:
        uint32_t _current_asserve;
        uint32_t _current_guidance;
        uint32_t _current_odometry;
        HoloControl * _holocontrol;
        Guidance    * _guidance;
        Odometry    * _odom;



    public:
        uint32_t get_guidance_state(){return _current_guidance;};
        uint32_t get_asserve_state(){return _current_asserve;};
        uint32_t get_odom_state(){return _current_odometry;};

        /*Default values :
        - ODOMETRY_ENABLED
        - GUIDANCE_BASIC
        - ASSERV_POS
        */
        void init(HoloControl* holo, Odometry* odom, Guidance* guidance){
            _holocontrol = holo;
            _guidance = guidance;
            _odom = odom;
            set_odometry((uint32_t)protoduck::System::OdometryFlags::ODOMETRY_ENABLED);
            set_guidance((uint32_t)protoduck::System::GuidanceFlags::GUIDANCE_BASIC);
            set_asserve((uint32_t)protoduck::System::AsservFlags::ASSERV_POS);
            }

        void set_asserve(uint32_t asserve){
            if (asserve & (uint32_t)protoduck::System::AsservFlags::ASSERV_SPEED) 
                {_holocontrol->enable_position_control(false);}
                
            if (asserve & (uint32_t)protoduck::System::AsservFlags::ASSERV_POS)
                {_holocontrol->enable_position_control(true);}
            
            _holocontrol->set_cons({0,0,0},{0,0,0});
            _guidance->set_target(_odom->get_pos());
            _current_asserve = asserve;
            }

        void set_guidance(uint32_t guidance){
            _holocontrol->set_cons({0,0,0},{0,0,0});
            _guidance->set_target(_odom->get_pos());
            _current_guidance = guidance;
            }

        void set_odometry(uint32_t odometry){
            _holocontrol->set_cons({0,0,0},{0,0,0});
            _guidance->set_target(_odom->get_pos());
            _current_odometry = odometry;
            }

};