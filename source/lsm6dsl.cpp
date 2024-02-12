#include "lsm6dsl.h"
#include "hal.h"
#include "stdutil.h"
#include "lms6dsl_reg.h"
#include "messages.h"
#include "communication.h"

#define LSM6DSL_SENSOR_ODR  LSM6DSL_ODR_833Hz
#define LSM6DSL_FIFO_ODR    LSM6DSL_FIFO_CTRL5_ODR_833Hz

imu_data_t imu_data_buffer[IMU_DATA_NB];
int imu_data_write_index = 0;
int imu_data_read_index = 0;


void data_cb(SPIDriver*);
void spi_error_cb(SPIDriver*);

#define BUF_SIZE 512

constexpr uint16_t NB_WORDS_MAX = ((BUF_SIZE-1) / 2) - ((BUF_SIZE-1) / 2)%6;
/*
 * SPI TX and RX buffers.
 */
CC_ALIGN_DATA(32) static uint8_t txbuf[BUF_SIZE] = {0};
CC_ALIGN_DATA(32) static uint8_t rxbuf[BUF_SIZE] = {0};

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
  //palToggleLine(LINE_LED1);
  spiUnselectI(&SPID1);
}

void spi_error_cb(SPIDriver*) {
  chSysHalt("SPI error");
}

void writeRegister(uint8_t addr, uint8_t val) {
  txbuf[0] = addr;
  txbuf[1] = val;
  spiSelect(&SPID1);
  spiExchange(&SPID1, 2, txbuf, rxbuf);
}

uint8_t readRegister(uint8_t addr) {
  txbuf[0] = addr | 0x80;
  spiSelect(&SPID1);
  spiExchange(&SPID1, 2, txbuf, rxbuf);
  return rxbuf[1];
}

void readData() {
  txbuf[0] = LSM6DSL_FIFO_DATA_OUT_L | 0x80;
  spiSelect(&SPID1);
  spiExchange(&SPID1, 3, txbuf, rxbuf);
}

void readFIFO() {
  // read FIFO_STATUS[1-4]
  txbuf[0] = LSM6DSL_FIFO_STATUS1 | 0x80;
  spiSelect(&SPID1);
  spiExchange(&SPID1, 5, txbuf, rxbuf);

  uint16_t nb_unread = ((rxbuf[2] & 0x07) << 8) | rxbuf[1];
  uint16_t pattern = ((rxbuf[4] & 0x03) << 8) | rxbuf[3];
  
  uint16_t words_to_read = nb_unread - nb_unread%6;
  if(words_to_read > NB_WORDS_MAX) {
    words_to_read = NB_WORDS_MAX;
  }

  if(words_to_read >= 6) {
    if(pattern != 0) {
      // read data pour se caler sur 0
      txbuf[0] = LSM6DSL_FIFO_DATA_OUT_L | 0x80;
      spiSelect(&SPID1);
      spiExchange(&SPID1, (6-pattern)*2+1, txbuf, rxbuf);
    } else {
      // read data
      txbuf[0] = LSM6DSL_FIFO_DATA_OUT_L | 0x80;
      spiSelect(&SPID1);
      spiExchange(&SPID1, words_to_read*2+1, txbuf, rxbuf);

      for(int i=0; i<words_to_read/6; i++) {
        imu_data_t* data = &imu_data_buffer[imu_data_write_index];

        data->gx  = *((int16_t*)&rxbuf[1+12*i + 0])  * 500.0 / powf(2, 15);
        data->gy  = *((int16_t*)&rxbuf[1+12*i + 2])  * 500.0 / powf(2, 15);
        data->gz  = *((int16_t*)&rxbuf[1+12*i + 4])  * 500.0 / powf(2, 15);
        data->xlx = *((int16_t*)&rxbuf[1+12*i + 6])  / powf(2, 15) * 2;
        data->xly = *((int16_t*)&rxbuf[1+12*i + 8])  / powf(2, 15) * 2;
        data->xlz = *((int16_t*)&rxbuf[1+12*i + 10]) / powf(2, 15) * 2;

        imu_data_write_index = (imu_data_write_index + 1) % IMU_DATA_NB;
      }
      palToggleLine(LINE_LED1);
    }
  }
}

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waImu, 512);
static void imu(void *) {
  chRegSetThreadName("imu");

  //reboot IMU
  writeRegister(LSM6DSL_CTRL3_C, 0x80);
  chThdSleepMilliseconds(20);

  while(true) {
    // wait for the WHO_AM_I to be read correctly
    uint8_t who_am_i = readRegister(LSM6DSL_WHO_AM_I_REG);
    if(who_am_i == 0x6A) {
        break;
    }
  }




  // configure IMU

  writeRegister(LSM6DSL_CTRL1_XL, LSM6DSL_SENSOR_ODR | LSM6DSL_ACC_FULLSCALE_2G);
  writeRegister(LSM6DSL_CTRL2_G, LSM6DSL_SENSOR_ODR | LSM6DSL_GYRO_FS_500);
  writeRegister(LSM6DSL_CTRL3_C, LSM6DSL_BDU_BLOCK_UPDATE | LSM6DSL_IF_INC_ENABLED);

  writeRegister(LSM6DSL_FIFO_CTRL1, 60);
  writeRegister(LSM6DSL_FIFO_CTRL2, 0);
  writeRegister(LSM6DSL_FIFO_CTRL3, LSM6DSL_GYRO_FIFO_CTRL3_DEC_1 | LSM6DSL_ACC_FIFO_CTRL3_DEC_1); //needed ?
  writeRegister(LSM6DSL_FIFO_CTRL4, 0);
  writeRegister(LSM6DSL_FIFO_CTRL5, LSM6DSL_FIFO_ODR | LSM6DSL_FIFO_CTRL5_MODE_CONTINUOUS);

  writeRegister(LSM6DSL_INT1_CTRL, LSM6DSL_INT1_CTRL_FTH | LSM6DSL_INT1_CTRL_FIFO_OVR | LSM6DSL_INT1_CTRL_FULL_FLAG);

  
  // enable interruptions on IMU_INT1
  palEnableLineEvent(LINE_IMU_INT1, PAL_EVENT_MODE_RISING_EDGE);

  if(palReadLine(LINE_IMU_INT1) == PAL_HIGH) {
    // FIFO may be full, reset it
    writeRegister(LSM6DSL_FIFO_CTRL5, LSM6DSL_FIFO_ODR | LSM6DSL_FIFO_CTRL5_MODE_BYPASS);
    writeRegister(LSM6DSL_FIFO_CTRL5, LSM6DSL_FIFO_ODR | LSM6DSL_FIFO_CTRL5_MODE_CONTINUOUS);
  }

  while(true) {
    msg_t ret = palWaitLineTimeout(LINE_IMU_INT1, chTimeMS2I(500));
    if(ret == MSG_OK) {
      readFIFO();
    } else {
      DebugTrace("no event :-( %lu", palReadLine(LINE_IMU_INT1));
      // reset FIFO (maybe it helps)
      writeRegister(LSM6DSL_FIFO_CTRL5, LSM6DSL_FIFO_ODR | LSM6DSL_FIFO_CTRL5_MODE_BYPASS);
      writeRegister(LSM6DSL_FIFO_CTRL5, LSM6DSL_FIFO_ODR | LSM6DSL_FIFO_CTRL5_MODE_CONTINUOUS);
    }
  }
}


void imuStart() {
    spiStart(&SPID1, &spi1conf);
    chThdCreateStatic(waImu, sizeof(waImu), NORMALPRIO, imu, NULL);
}

