#include "voltage_monitor.h"

#include "ch.h"
#include "hal.h"
#include "stdutil.h"
#include "printf.h"
#include "messages.h"
#include "communication.h"


#define ADC_TO_VOLT 189.2

#define ADC_GRP1_NUM_CHANNELS 1
#define ADC_GRP1_BUF_DEPTH    1


void adcerrorcallback(ADCDriver *adcp, adcerror_t err);

const ADCConfig portab_adccfg1 = {
  .difsel       = 0U
};

/*
 * ADC conversion group 1.
 * Mode:        One shot, 1 channels, SW triggered.
 * Channels:    IN1.
 */
const ADCConversionGroup portab_adcgrpcfg1 = {
  .circular     = false,
  .num_channels = ADC_GRP1_NUM_CHANNELS,
  .end_cb       = NULL,
  .error_cb     = adcerrorcallback,
  .cfgr         = 0U,
  .cfgr2        = 0U,
  .tr1          = ADC_TR_DISABLED,
  .tr2          = ADC_TR_DISABLED,
  .tr3          = ADC_TR_DISABLED,
  .awd2cr       = 0U,
  .awd3cr       = 0U,
  .smpr         = {
    ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_247P5),
    0U
  },
  .sqr          = {
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN1),
    0U,
    0U,
    0U
  }
};



/*
 * ADC errors callback, should never happen.
 */
void adcerrorcallback(ADCDriver *adcp, adcerror_t err) {

  (void)adcp;
  (void)err;

  chSysHalt("it happened");
}


#if CACHE_LINE_SIZE > 0
CC_ALIGN_DATA(CACHE_LINE_SIZE)
#endif
adcsample_t samples1[CACHE_SIZE_ALIGN(adcsample_t, ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH)];



/*
 * ADC readings
 */
static THD_WORKING_AREA(waThreadADC, 512);
static THD_FUNCTION(ThreadADC, arg) {
  (void)arg;
  chRegSetThreadName("ADC reading");

  adcStart(&ADCD5, &portab_adccfg1);

  while (true) {
    msg_t status = adcConvert(&ADCD5, &portab_adcgrpcfg1, samples1, ADC_GRP1_BUF_DEPTH);
    cacheBufferInvalidate(samples1, sizeof (samples1) / sizeof (adcsample_t));
    if(status == MSG_OK) {
      double voltage = samples1[0]/189.2;
      Message msg;
      msg.mutable_bat().set_voltage(voltage);
      post_message(msg, Message::MsgType::STATUS, chTimeMS2I(500));
    }


    chThdSleepMilliseconds(2000);
  }
}

void voltageMonitorStart() {
  chThdCreateStatic(waThreadADC, sizeof(waThreadADC), NORMALPRIO, ThreadADC, NULL);
}
