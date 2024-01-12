#include "voltage_monitor.h"

#include "ch.h"
#include "hal.h"
#include "usbcfg.h"
#include "stdutil.h"
#include "printf.h"


void adcerrorcallback(ADCDriver *adcp, adcerror_t err);

const ADCConfig portab_adccfg1 = {
  .difsel       = 0U
};


// #define VC1_ADC_IN	 7
// #define VC2_ADC_IN	 8
// #define VC3_ADC_IN	 9
// #define VC4_ADC_IN	 1
// #define VC5_ADC_IN	 2
// #define VC6_ADC_IN	 3

#define ADC_GRP1_NUM_CHANNELS 6
#define ADC_GRP1_BUF_DEPTH      1

/*
 * ADC conversion group 1.
 * Mode:        One shot, 2 channels, SW triggered.
 * Channels:    IN1, IN2.
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
    ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_247P5) |
    ADC_SMPR1_SMP_AN2(ADC_SMPR_SMP_247P5) |
    ADC_SMPR1_SMP_AN3(ADC_SMPR_SMP_247P5) |
    ADC_SMPR1_SMP_AN7(ADC_SMPR_SMP_247P5) |
    ADC_SMPR1_SMP_AN8(ADC_SMPR_SMP_247P5) |
    ADC_SMPR1_SMP_AN9(ADC_SMPR_SMP_247P5),
    0U
  },
  .sqr          = {
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN7) |
    ADC_SQR1_SQ2_N(ADC_CHANNEL_IN8) |
    ADC_SQR1_SQ3_N(ADC_CHANNEL_IN9),
    ADC_SQR2_SQ7_N(ADC_CHANNEL_IN1) |
    ADC_SQR2_SQ8_N(ADC_CHANNEL_IN2) |
    ADC_SQR2_SQ9_N(ADC_CHANNEL_IN3),
    0U,
    0U
  }
};










// void adccallback(ADCDriver *adcp) {

//   /* Updating counters.*/
//   n++;
//   if (adcIsBufferComplete(adcp)) {
//     nx += 1;
//   }
//   else {
//     ny += 1;
//   }

//   if ((n % 200) == 0U) {
// #if defined(PORTAB_LINE_LED2)
//     palToggleLine(PORTAB_LINE_LED2);
// #endif
//   }
// }

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

  adcStart(&ADCD1, &portab_adccfg1);
  //adcSTM32EnableVREF(&ADCD1);


  while (true) {
    /* Performing a one-shot conversion on two channels.*/
    msg_t status = adcConvert(&ADCD1, &portab_adcgrpcfg1, samples1, ADC_GRP1_BUF_DEPTH);
    cacheBufferInvalidate(samples1, sizeof (samples1) / sizeof (adcsample_t));
    if(status == MSG_OK) {
      //chprintf ((BaseSequentialStream*)&PORTAB_SDU1, "%u\r\n", samples1[0]);
    }


    chThdSleepMilliseconds(200);
  }
}

void start_adc_thread() {
  chThdCreateStatic(waThreadADC, sizeof(waThreadADC), NORMALPRIO, ThreadADC, NULL);
}