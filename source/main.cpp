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

#define PWM_FREQ   50000
#define PWM_PERIOD 250

PWMConfig pwmcfg1 = {
  .frequency = PWM_FREQ,
  .period = PWM_PERIOD,
  .callback = NULL,
  .channels = {
    {                                                               //ch1
      .mode = PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,
      .callback = NULL,
    },
    {                                                               //ch2
      .mode = PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,
      .callback = NULL
    },
    {                                                               //ch3
      .mode = PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,
      .callback = NULL
    },
    {                                                               //ch4
      .mode = PWM_OUTPUT_DISABLED,
      .callback = NULL
    },
  },
  .cr2 = 0,
  .bdtr = 0,
  .dier = 0
};

static pwmcnt_t getPwmCnt(float speed) {
  if(fabs(speed) > 100.0) {
    speed = 100.0;
  } else {
    speed = fabs(speed);
  }
  return (pwmcnt_t)((100.0 - speed)/100.0 * PWM_PERIOD);
}


/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 512);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;
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
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread2, 512);
static THD_FUNCTION(Thread2, arg) {
  (void)arg;
  chRegSetThreadName("pwm test");
    palWriteLine(LINE_MOT_EN, PAL_LOW);
    palWriteLine(LINE_MOT1_DIR, PAL_LOW);
    palWriteLine(LINE_MOT2_DIR, PAL_HIGH);
  while (true) {
    for(int i=0; i<100; i++) {
      pwmcnt_t width = getPwmCnt(i);
      pwmEnableChannel(&PWMD1, 1, width);
      pwmEnableChannel(&PWMD1, 2, width);
      chThdSleepMilliseconds(50);
    }
    for(int i=100; i>0; i--) {
      pwmcnt_t width = getPwmCnt(i);
      pwmEnableChannel(&PWMD1, 1, width);
      pwmEnableChannel(&PWMD1, 2, width);
      chThdSleepMilliseconds(50);
    }
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
  sdStart(&SD4, NULL);
  pwmStart(&PWMD1, &pwmcfg1);

  consoleInit();  // initialisation des objets liés au shell

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

  // cette fonction en interne fait une boucle infinie, elle ne sort jamais
  // donc tout code situé après ne sera jamais exécuté.
  consoleLaunch();  // lancement du shell

  // main thread does nothing
  chThdSleep(TIME_INFINITE);
}

