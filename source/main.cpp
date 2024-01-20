/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "voltage_monitor.h"
#include "printf.h"
#include "ttyConsole.h"
#include "stdutil.h"
#include "usb_serial.h"
#include <math.h>
#include "lsm6dsl.h"
#include "globalVar.h"
#include "communication.h"

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





/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();
  initHeap();

  consoleInit();  // initialisation des objets liés au shell

  imuStart();
  enc1.init(false);
  enc2.init(false);
  enc3.init(false);

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

  start_communication();

  pwmStart(&PWMD1, &pwmcfg1);


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

