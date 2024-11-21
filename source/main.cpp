#include "ch.h"
#include "hal.h"
#include "printf.h"
#include "ttyConsole.h"
#include "stdutil.h"
#include "usb_serial.h"
#include <math.h>
#include "lsm6dsl.h"
#include "globalVar.h"
#include "communication.h"
#include "motor.h"
#include "holocontrol.h"
#include "BytesReadBuffer.h"
#include "BytesWriteBuffer.h"
#include <Eigen/Core>
#include "messages.h"
#include "communication/telemetry.h"
#include "guidance.h"
#include "voltage_monitor.h"
#include "ins.h"


/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waBlinker, 512);
static void blinker(void *) {
  chRegSetThreadName("blinker");
  while (true) {
    palClearLine(LINE_LED2);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED2);
    chThdSleepMilliseconds(500);
    //DebugTrace("%s", buf);
  }
}


static THD_WORKING_AREA(encodersFilter, 1024);
static void encFilter(void *) {
  chRegSetThreadName("encodersFilter");
  while (true) {
    systime_t now = chVTGetSystemTime();
    odometry.update_filters();
    chThdSleepUntil(chTimeAddX(now,chTimeMS2I(ENCODERS_PERIOD)));
  }
}

static THD_WORKING_AREA(locomotion, 5000);
static void locomth(void *) {
  chRegSetThreadName("locomth");
  while (true) {
    systime_t now = chVTGetSystemTime();
    
    if (systemmanager.get_odom_state()& (uint32_t)Syst::OdometryFlags::ODOMETRY_ENABLED)
      {odometry.update();}
    
    if (systemmanager.get_guidance_state()& (uint32_t)Syst::GuidanceFlags::GUIDANCE_BASIC)
      {guidance.update();}

    if (systemmanager.get_control_state() == 0) {
       // No asserve
      } else if (systemmanager.get_control_state() & (uint32_t)Syst::AsservFlags::ASSERV_DIRECT) {
       // directcontrol.update();
      } else if (systemmanager.get_control_state() & ((uint32_t)Syst::AsservFlags::ASSERV_SPEED | (uint32_t)Syst::AsservFlags::ASSERV_POS)) {
      holocontrol.update();
      }

    palToggleLine(LINE_LED1);
    //DebugTrace("plop");
    
    chThdSleepUntil(chTimeAddX(now,chTimeMS2I(ODOM_PERIOD)));
  }
}



void pos_cons_cb(protoduck::Message& msg) {
   if(msg.get_msg_type() == protoduck::Message::MsgType::COMMAND &&
      msg.has_pos()) {
      if(msg.get_pos().get_obj() == protoduck::Pos::PosObject::POS_ROBOT_W) {
        Eigen::Vector3d pos {msg.get_pos().get_x(),msg.get_pos().get_y(),msg.get_pos().get_theta()};
        if(systemmanager.get_guidance_state() & (uint32_t)Syst::GuidanceFlags::GUIDANCE_BASIC)
          {guidance.set_target(pos);}
        //holocontrol.set_cons(pos,{0,0,0});

      } else if(msg.get_pos().get_obj() == protoduck::Pos::PosObject::RECALAGE) {
        auto x = msg.get_pos().get_x();
        auto y = msg.get_pos().get_y();
        auto theta = msg.get_pos().get_theta();

        if (systemmanager.get_odom_state()& (uint32_t)Syst::OdometryFlags::ODOMETRY_ENABLED) {
          odometry.set_pos(x, y, theta);}

        if (systemmanager.get_odom_state()& (uint32_t)Syst::OdometryFlags::ODOMETRY_INS_ON) {
          ins_set_theta(theta);}
        
      }
   }
}

void system_ctl_cb(protoduck::Message& msg) {
  if(msg.get_msg_type() == protoduck::Message::MsgType::COMMAND &&
      msg.has_system()) {
        systemmanager.set_control(msg.get_system().get_asserv());
        systemmanager.set_guidance(msg.get_system().get_guidance());
        systemmanager.set_odometry(msg.get_system().get_odometry());
      }
}

void speed_cons_cb(protoduck::Message& msg) {
   if(msg.get_msg_type() == protoduck::Message::MsgType::COMMAND &&
      msg.has_speed()) {
        Eigen::Vector3d speedW;
        Eigen::Vector3d speedR;


        auto vx = msg.get_speed().get_vx();
        auto vy = msg.get_speed().get_vy();
        auto vtheta = msg.get_speed().get_vtheta();

        speedW[0] = vx;
        speedW[1] = vy;
        speedW[2] = vtheta;

        if (systemmanager.get_odom_state()& (uint32_t)Syst::OdometryFlags::ODOMETRY_ENABLED){
          auto theta = odometry.get_theta();
          
          const Eigen::Matrix<double, 3, 3> rot {
              {cos(theta) , sin(theta), 0},
              {-sin(theta), cos(theta), 0},
              {0          ,          0, 1}};
          
          speedR = rot * (speedW);
          }

        if (systemmanager.get_control_state() & ((uint32_t)Syst::AsservFlags::ASSERV_SPEED | (uint32_t)Syst::AsservFlags::ASSERV_POS)) {
          if (systemmanager.get_odom_state()& (uint32_t)Syst::OdometryFlags::ODOMETRY_ENABLED){
            holocontrol.set_cons({0,0,0}, speedR);}
          }
        //DebugTrace("v: %f %f %f", vx, vy, vtheta);
   }
}



void pid_cons_cb(protoduck::Message& msg) {
   if(msg.get_msg_type() == protoduck::Message::MsgType::COMMAND &&
      msg.has_motor_pid()) {
        auto pids = msg.get_motor_pid();
        holocontrol.set_vel_pid_gains(pids.get_kp(), pids.get_ki(), pids.get_kd());
   }
}



int main(void) {
  // ChibiOS init
  halInit();
  chSysInit();
  initHeap();

  consoleInit();  // initialisation des objets liés au shell

  // subsystems init

  odometry.init();
  holocontrol.init();
  guidance.init();
  systemmanager.init(&holocontrol);

  imuStart();
  insStart();
  communicationStart();
  telemetryStart();
  voltageMonitorStart();
  

  register_callback(pos_cons_cb);
  register_callback(pid_cons_cb);
  register_callback(speed_cons_cb);
  register_callback(system_ctl_cb);


  
  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker, NULL);

  chThdCreateStatic(encodersFilter, sizeof(encodersFilter), NORMALPRIO+1, encFilter, NULL);
  chThdCreateStatic(locomotion, sizeof(locomotion), NORMALPRIO+1, locomth, NULL);



  consoleLaunch();  // launch shell. Never returns.

  chThdSleep(TIME_INFINITE);
}

