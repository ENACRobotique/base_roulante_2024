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


// #if defined(BOARD_DC)
// #error "board DC"
// #elif defined(BOARD_CAN)
// #error "board CAN"
// #else
// #error "No board defined"
// #endif


/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waBlinker, 512);
[[noreturn]]
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

#if defined(BOARD_DC)

static THD_WORKING_AREA(encodersFilter, 1024);
[[noreturn]]
static void encFilter(void *) {
  chRegSetThreadName("encodersFilter");
  while (true) {
    systime_t now = chVTGetSystemTime();
    odometry.update_filters();
    chThdSleepUntil(chTimeAddX(now,chTimeMS2I(ENCODERS_PERIOD)));
  }
}

#endif

static THD_WORKING_AREA(locomotion, 5000);
[[noreturn]]
static void locomth(void *) {
  chRegSetThreadName("locomth");
  while (true) {
    systime_t now = chVTGetSystemTime();
    
    odometry.update();
    //   guidance.update();
    holocontrol.update();

    //DebugTrace("plop");
    
    chThdSleepUntil(chTimeAddX(now,chTimeMS2I(ODOM_PERIOD)));
  }
}



void pos_cons_cb(protoduck::Message& msg) {
   if(msg.get_msg_type() == protoduck::Message::MsgType::COMMAND &&
      msg.has_pos()) {
      if(msg.get_pos().get_obj() == protoduck::Pos::PosObject::POS_ROBOT_W) {
        Eigen::Vector3d pos {msg.get_pos().get_x(),msg.get_pos().get_y(),msg.get_pos().get_theta()};
        guidance.set_target(pos);

      } else if(msg.get_pos().get_obj() == protoduck::Pos::PosObject::RECALAGE) {
        auto x = msg.get_pos().get_x();
        auto y = msg.get_pos().get_y();
        auto theta = msg.get_pos().get_theta();

        
        odometry.set_pos(x, y, theta);
        ins_set_theta(-theta);
        
      }
   }
}

void speed_cons_cb(protoduck::Message& msg) {
   if(msg.get_msg_type() == protoduck::Message::MsgType::COMMAND &&
      msg.has_speed()) {
        Eigen::Vector3d speedR;


        auto vx = msg.get_speed().get_vx();
        auto vy = msg.get_speed().get_vy();
        auto vtheta = msg.get_speed().get_vtheta();

        speedR[0] = vx;
        speedR[1] = vy;
        speedR[2] = vtheta;
        
        holocontrol.set_cons({0,0,0}, speedR);

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


void _init_chibios() __attribute__ ((constructor(101)));
void _init_chibios() {
  halInit();
  chSysInit();
  initHeap ();
}


int main(void) {
  consoleInit();  // initialisation des objets liés au shell
  

  // subsystems init
  odometry.init();
  holocontrol.init();
  guidance.init();

  imuStart();
  insStart();
  communicationStart();
  telemetryStart();
  voltageMonitorStart();
  

  register_callback(pos_cons_cb);
  register_callback(pid_cons_cb);
  register_callback(speed_cons_cb);


  chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker, NULL);
  chThdCreateStatic(locomotion, sizeof(locomotion), NORMALPRIO+1, locomth, NULL);

  #if defined(BOARD_DC)
  chThdCreateStatic(encodersFilter, sizeof(encodersFilter), NORMALPRIO+1, encFilter, NULL);
  #endif

  consoleLaunch();
  

  chThdSleep(TIME_INFINITE);
}

