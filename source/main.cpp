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
#include "controlHolo.h"
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

Position lidar_pos;
bool rcv_lidar = false;

static THD_WORKING_AREA(locomotion, 5000);
[[noreturn]]
static void locomth(void *) {
  chRegSetThreadName("locomth");
  while (true) {
    systime_t now = chVTGetSystemTime();
    //DebugTrace("plop");
    odometry.update();
    guidance.update();
    control.update();
    chThdSleepUntil(chTimeAddX(now,chTimeMS2I(ODOM_PERIOD_MS)));
  }
}

static THD_WORKING_AREA(Wa_ekf_predict, 10000);
[[noreturn]]
static void ekf_predict(void *) {
  chRegSetThreadName("ekf_predict");
  int c = 0;
  while (true) {
    systime_t now = chVTGetSystemTime();

    ekf.predict();

    if(c % 2 == 0) {
      ekf.update_gyro(ins_get_vtheta());
      auto speed = odometry.get_speed();
      ekf.update_encoders(speed.vx(), speed.vtheta());
    }
    
    if(rcv_lidar) {
      rcv_lidar = false;
      ekf.update_lidar(lidar_pos.x(), lidar_pos.y(), lidar_pos.theta());
    }
    
    
    chThdSleepUntil(chTimeAddX(now,chTimeMS2I(20)));
    c += 1;
  }
}


void pos_cons_cb(e::Message<MOTORS_NB>& msg) {
   if(msg.get_msg_type() == e::Message<MOTORS_NB>::MsgType::COMMAND &&
      msg.has_pos()) {
      if(msg.get_topic() == e::Topic::POS_ROBOT_W) {
        auto pos = Position(msg.get_pos());
        //Position pos = Position(msg.get_pos().get_x(),msg.get_pos().get_y(),msg.get_pos().get_theta());
        guidance.set_target(pos);

      } else if(msg.get_topic() == e::Topic::RECALAGE) {
        auto theta = msg.get_pos().get_theta();
        odometry.set_pos(msg.get_pos());
        ins_set_theta(-theta);
      }
   }
}

void speed_cons_cb(e::Message<MOTORS_NB>& msg) {
   if(msg.get_msg_type() == e::Message<MOTORS_NB>::MsgType::COMMAND &&
      msg.has_speed()) {

        // if guidance is running, abort
        guidance.abort();

        auto vx = msg.get_speed().get_vx();
        auto vy = msg.get_speed().get_vy();
        auto vtheta = msg.get_speed().get_vtheta();

        Speed speedR = Speed(vx,vy,vtheta);

        control.set_cons(speedR);

        //DebugTrace("v: %f %f %f", vx, vy, vtheta);
   }
}



void lidar_pos_cb(e::Message<MOTORS_NB>& msg) {
  if(msg.get_msg_type() == e::Message<MOTORS_NB>::MsgType::STATUS && msg.has_pos() && msg.get_topic() == e::Topic::POS_LIDAR) {
    auto pos = msg.get_pos();
    lidar_pos = Position(msg.get_pos());
    rcv_lidar = true;
    //ekf.update_lidar(pos.get_x(), pos.get_y(), pos.get_theta());
  }
}



void pid_cons_cb(e::Message<MOTORS_NB>& msg) {
   if(msg.get_msg_type() == e::Message<MOTORS_NB>::MsgType::COMMAND &&
      msg.has_motor_pid()) {
        auto pids = msg.get_motor_pid();
        if(pids.motor_no() == 0) {
          control.set_vit_d_pid_gains(pids.get_kp(), pids.get_ki(), pids.get_kd());
        }
        else if(pids.motor_no() == 1) {
          control.set_vit_ang_pid_gains(pids.get_kp(), pids.get_ki(), pids.get_kd());
        }
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
  control.init();
  guidance.init();
  ekf.init(0.02);

  imuStart();
  insStart();
  communicationStart();
  telemetryStart();
  voltageMonitorStart();
  

  register_callback(pos_cons_cb);
  // register_callback(traj_cons_cb);
  register_callback(pid_cons_cb);
  register_callback(speed_cons_cb);
  register_callback(lidar_pos_cb);


  chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker, NULL);
  chThdCreateStatic(locomotion, sizeof(locomotion), NORMALPRIO+1, locomth, NULL);
  chThdCreateStatic(Wa_ekf_predict, sizeof(Wa_ekf_predict), NORMALPRIO+1, ekf_predict, NULL);

  #if defined(BOARD_DC)
  chThdCreateStatic(encodersFilter, sizeof(encodersFilter), NORMALPRIO+1, encFilter, NULL);
  #endif

  consoleLaunch();
  

  chThdSleep(TIME_INFINITE);
}

