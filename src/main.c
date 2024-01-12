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
#include "rt_test_root.h"
#include "oslib_test_root.h"
#include "usbcfg.h"
#include "voltage_monitor.h"
#include "printf.h"
/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  chRegSetThreadName("blinker");
  const char buf[] = "plip\r\n";
  while (true) {
    palClearLine(LINE_LED2);
    chThdSleepMilliseconds(200);
    palSetLine(LINE_LED2);
    chThdSleepMilliseconds(200);
//    chprintf ((BaseSequentialStream*)&PORTAB_SDU1, "%s", buf);
    chnWrite(&PORTAB_SDU1, (uint8_t*) buf, sizeof buf - 1);
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

//  palSetLine(LINE_POWER_KEY);

  /*
   * Activates the Serial or SIO driver using the default configuration.
   */
  sdStart(&SD4, NULL);


  sduObjectInit(&PORTAB_SDU1);
  sduStart(&PORTAB_SDU1, &serusbcfg);



  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);





  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  //start_adc_thread();

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
  //  if (true) {
  //     test_execute((BaseSequentialStream *)&PORTAB_SDU1, &rt_test_suite);
  //     test_execute((BaseSequentialStream *)&PORTAB_SDU1, &oslib_test_suite);
  //   }
    chThdSleepMilliseconds(2000);
  }
}

