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
        _current_asserve = (uint32_t)(protoduck::System::AsservFlags::ASSERV_POS);
        _current_guidance = (uint32_t)protoduck::System::GuidanceFlags::GUIDANCE_BASIC;
        _current_odometry = (uint32_t)protoduck::System::OdometryFlags::ODOMETRY_ENABLED;
        }
    //Reminder : remember when choosing ASSERV_SPEED pos_cons won't work !
    void set_asserve(uint32_t asserve){
        if (asserve == (uint32_t)protoduck::System::AsservFlags::ASSERV_NONE) {
            _holocontrol->enable_asserve(false);
        } else {
            _holocontrol->enable_asserve(true);
        }
            
        if (asserve & (uint32_t)protoduck::System::AsservFlags::ASSERV_POS) {
            _holocontrol->enable_position_control(true);
        } else {
            _holocontrol->enable_position_control(false);
        }

        stop_motors();        
        _guidance->set_target(_odom->get_pos());
        _guidance->update();

        _current_asserve = asserve;
        }

    void set_guidance(uint32_t guidance){
        stop_motors();
        _guidance->set_target(_odom->get_pos());
        _guidance->update();
        _current_guidance = guidance;
        }

    void set_odometry(uint32_t odometry){
        stop_motors();
        _guidance->set_target(_odom->get_pos());
        _guidance->update();
        _current_odometry = odometry;
        }

    void stop_motors();
};