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

static THD_WORKING_AREA(odom, 2048);
static void odomth(void *) {
  chRegSetThreadName("odomth");
  while (true) {
    systime_t now = chVTGetSystemTime();
    odometry.update();
    
    chThdSleepUntil(chTimeAddX(now,chTimeMS2I(ODOM_PERIOD)));
  }
}

 static THD_WORKING_AREA(holo, 8192);
 static void holoth(void *) {
   chRegSetThreadName("holoth");
   while (true) {
     systime_t now = chVTGetSystemTime();  
     holocontrol.update();

  
    
     chThdSleepUntil(chTimeAddX(now,chTimeMS2I(ODOM_PERIOD)));
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
  imuStart();


  
  
  communicationStart();

 

  const Eigen::Vector3d pos {1000,0,0};  // 2pi = 1 tour
  const Eigen::Vector3d vitesse {20,0,0};


  // communication callback example
  // TODO: remove this
  //register_callback([](protoduck::Message msg) {
  //  (void)msg;
  //  palToggleLine(LINE_LED1);
  //  msg.clear();
  //  auto& pos = msg.mutable_pos();
  //  pos.set_x(enc1.get_value());
  //  pos.set_y(enc2.get_value());
  //  pos.set_theta(enc3.get_value());
  //  post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);
  //});


  
  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker, NULL);

  chThdCreateStatic(encodersFilter, sizeof(encodersFilter), NORMALPRIO+1, encFilter, NULL);
  chThdCreateStatic(odom, sizeof(odom), NORMALPRIO+1, odomth, NULL);
  chThdCreateStatic(holo, sizeof(holo), NORMALPRIO+1, holoth, NULL);


  holocontrol.set_cons(pos,vitesse);

  

  // cette fonction en interne fait une boucle infinie, elle ne sort jamais
  // donc tout code situé après ne sera jamais exécuté.
  consoleLaunch();  // lancement du shell


  // main thread does nothing
  chThdSleep(TIME_INFINITE);
}

