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

#include "BytesReadBuffer.h"
#include "BytesWriteBuffer.h"

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





int main(void) {
  // ChibiOS init
  halInit();
  chSysInit();
  initHeap();

  consoleInit();  // initialisation des objets liés au shell

  // subsystems init
  odometry.init();
  imuStart();
  motorsStart();
  communicationStart();


  // communication callback example
  // TODO: remove this
  register_callback([](protoduck::Message msg) {
    (void)msg;
    palToggleLine(LINE_LED1);
    msg.clear();
    auto& pos = msg.mutable_pos();
    pos.set_x(enc1.get_value());
    pos.set_y(enc2.get_value());
    pos.set_theta(enc3.get_value());
    post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);
  });


  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker, NULL);
  
  // cette fonction en interne fait une boucle infinie, elle ne sort jamais
  // donc tout code situé après ne sera jamais exécuté.
  consoleLaunch();  // lancement du shell

  // main thread does nothing
  chThdSleep(TIME_INFINITE);
}

