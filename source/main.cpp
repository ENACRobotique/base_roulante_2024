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

#include "BytesReadBuffer.h"
#include "BytesWriteBuffer.h"

#include "messages.h"


SerialConfig sd4conf = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = USART_CR2_STOP1_BITS | USART_CR2_LINEN,
  .cr3 = 0
};

// static pwmcnt_t getPwmCnt(float speed) {
//   if(fabs(speed) > 100.0) {
//     speed = 100.0;
//   } else {
//     speed = fabs(speed);
//   }
//   return (pwmcnt_t)((speed)/100.0 * PWM_PERIOD);
// }


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
 * test PWM
 */
static THD_WORKING_AREA(waPwmTest, 512);
static void pwmTest(void*) {
  chRegSetThreadName("pwm test");
    palWriteLine(LINE_MOT_EN, PAL_LOW);
    palWriteLine(LINE_MOT1_DIR, PAL_LOW);
    palWriteLine(LINE_MOT2_DIR, PAL_HIGH);
  while (true) {
    for(int i=0; i<10; i++) {
      // pwmcnt_t width = getPwmCnt(i);
      // pwmEnableChannel(&PWMD1, 1, width);
      // pwmEnableChannel(&PWMD1, 2, width);
      chThdSleepMilliseconds(100);
    }
    for(int i=10; i>0; i--) {
      // pwmcnt_t width = getPwmCnt(i);
      // pwmEnableChannel(&PWMD1, 1, width);
      // pwmEnableChannel(&PWMD1, 2, width);
      chThdSleepMilliseconds(100);
    }
  }
}


static THD_WORKING_AREA(waCom, 512);
static void com(void*) {
  chRegSetThreadName("com");
  
  BytesWriteBuffer buffer;
  protoduck::Message msg;
  auto& battery_report = msg.mutable_bat();
  
  float voltage = 10;

  while(true) {
    voltage += 0.2;
    battery_report.set_voltage(voltage);
    buffer.clear();
    msg.serialize(buffer);
    //chprintf((BaseSequentialStream*) &SD4, "plop\r\n");
    sdWrite(&SD4, buffer.get_data(), buffer.get_size());
    chThdSleepMilliseconds(1000);
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

  /*
   * Activates the Serial or SIO driver using the default configuration.
   */
  sdStart(&SD4, &sd4conf);
  pwmStart(&PWMD1, &pwmcfg1);

  consoleInit();  // initialisation des objets liés au shell

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker, NULL);
  chThdCreateStatic(waPwmTest, sizeof(waPwmTest), NORMALPRIO, pwmTest, NULL);
  chThdCreateStatic(waCom, sizeof(waCom), NORMALPRIO, com, NULL);
  
  imuStart();
  enc1.init(false);
  enc2.init(false);
  enc3.init(false);

  // cette fonction en interne fait une boucle infinie, elle ne sort jamais
  // donc tout code situé après ne sera jamais exécuté.
  consoleLaunch();  // lancement du shell

  // main thread does nothing
  chThdSleep(TIME_INFINITE);
}

