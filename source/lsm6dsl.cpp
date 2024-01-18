#include "lsm6dsl.h"
#include "hal.h"
#include "stdutil.h"


void data_cb(SPIDriver*);
void spi_error_cb(SPIDriver*);


/*
 * SPI TX and RX buffers.
 */
CC_ALIGN_DATA(32) static uint8_t txbuf[512] = {0};
CC_ALIGN_DATA(32) static uint8_t rxbuf[512] = {0};

SPIConfig spi1conf = {
  .circular = false,
  .slave = false,
  .data_cb = data_cb,
  .error_cb = spi_error_cb,
  .ssport = PAL_PORT(LINE_SPI1_CS),
  .sspad = PAL_PAD(LINE_SPI1_CS),
  .cr1= SPI_CR1_BR | SPI_CR1_CPOL | SPI_CR1_CPHA,
  .cr2 = 0
};


void data_cb(SPIDriver*) {
  palToggleLine(LINE_LED1);
  spiUnselectI(&SPID1);
}

void spi_error_cb(SPIDriver*) {
  chSysHalt("SPI error");
}


/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waImu, 512);
static void imu(void *) {
  chRegSetThreadName("imu");
  txbuf[0] = 0x0F | 0x80;

  while(true) {
    spiSelect(&SPID1);
    spiExchange(&SPID1, 2, txbuf, rxbuf);
    uint8_t who_am_i = rxbuf[1];
    DebugTrace("who_am_i: %x", who_am_i);
    
    chThdSleepMilliseconds(1000);
  }
}


void imuStart() {
    spiStart(&SPID1, &spi1conf);
    chThdCreateStatic(waImu, sizeof(waImu), NORMALPRIO, imu, NULL);
}

