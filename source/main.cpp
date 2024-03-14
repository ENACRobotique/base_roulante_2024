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

static THD_WORKING_AREA(locomotion, 4096);
static void locomth(void *) {
  chRegSetThreadName("locomth");
  while (true) {
    systime_t now = chVTGetSystemTime();
    odometry.update();
    guidance.update();
    holocontrol.update();
    
    chThdSleepUntil(chTimeAddX(now,chTimeMS2I(ODOM_PERIOD)));
  }
}



void pos_cons_cb(protoduck::Message& msg) {
   if(msg.get_msg_type() == protoduck::Message::MsgType::COMMAND &&
      msg.has_pos()) {
      if(msg.get_pos().get_obj() == protoduck::Pos::PosObject::POS_ROBOT_W) {
        Eigen::Vector3d pos {msg.get_pos().get_x(),msg.get_pos().get_y(),msg.get_pos().get_theta()};
        guidance.set_target(pos);
        //holocontrol.set_cons(pos,{0,0,0});

      } else if(msg.get_pos().get_obj() == protoduck::Pos::PosObject::RECALAGE) {
        auto x = msg.get_pos().get_x();
        auto y = msg.get_pos().get_y();
        auto theta = msg.get_pos().get_theta();
        odometry.set_pos(x, y, theta);
        ins_set_theta(theta);
      }
   }
}



void pid_cons_cb(protoduck::Message& msg) {
   if(msg.get_msg_type() == protoduck::Message::MsgType::COMMAND &&
      msg.has_motor_pid()) {
        auto pids = msg.get_motor_pid();
        holocontrol.set_pid_gains(pids.get_kp(), pids.get_ki(), pids.get_kd());
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

  imuStart();
  insStart();
  communicationStart();
  telemetryStart();
  voltageMonitorStart();
  

  register_callback(pos_cons_cb);
  register_callback(pid_cons_cb);


  
  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker, NULL);

  chThdCreateStatic(encodersFilter, sizeof(encodersFilter), NORMALPRIO+1, encFilter, NULL);
  chThdCreateStatic(locomotion, sizeof(locomotion), NORMALPRIO+1, locomth, NULL);



  consoleLaunch();  // launch shell. Never returns.

  chThdSleep(TIME_INFINITE);
}

