#include "motor_can.h"


#include <ch.h>
#include <hal.h>
#include <cstdint>
#include "globalVar.h"

#if defined(BOARD_CAN)

CANDriver* CAND = &CAND2;


static const CANConfig cancfg = {
    .op_mode = OPMODE_CAN,
    .NBTP = 1342187562,
    .DBTP = 8389922,
    .TDCR = 1540,
    .CCCR = 0,//FDCAN_CONFIG_CCCR_BRSE, // Bit-Rate Switch Enable
    .TEST = 0,

    .RXGFC = 0,
    // .RXGFC = FDCAN_CONFIG_RXGFC_RRFS | FDCAN_CONFIG_RXGFC_ANFS_REJECT |
    //          FDCAN_CONFIG_RXGFC_ANFE_REJECT
    
};

struct can_instance {
  CANDriver   *canp;
  std::array<MotorCAN, 4>* motors;
};

static const struct can_instance can2 = {
  .canp = &CAND2,
  .motors = &motors,
};


/*
 * Receiver thread.
 */
static THD_WORKING_AREA(can_rx_wa, 512);
static THD_FUNCTION(can_rx, p) {
  struct can_instance *cip = (struct can_instance*)p;
  //event_listener_t el;
  CANRxFrame rxmsg;

  (void)p;
  chRegSetThreadName("receiver");
  // chEvtRegister(&CAND->rxfull_event, &el, 0);
  while (true) {
    // if (chEvtWaitAnyTimeout(ALL_EVENTS, TIME_MS2I(100)) == 0)
    //   continue;
    while (canReceiveTimeout(CAND, CAN_ANY_MAILBOX, &rxmsg, TIME_INFINITE) == MSG_OK) {
      //chSysHalt("can msg available!");
      if(rxmsg.std.SID > 0x200 && rxmsg.std.SID <= 0x204) {
        size_t motor_id = rxmsg.std.SID - 0x201;

        int16_t angle  = (int16_t)__builtin_bswap16(rxmsg.data16[0]);
        int16_t speed  = (int16_t)__builtin_bswap16(rxmsg.data16[1]);
        int16_t torque = (int16_t) __builtin_bswap16(rxmsg.data16[2]);
        cip->motors->at(motor_id).set_status(angle, speed, torque);
      }
    }
  }
  // chEvtUnregister(&CAND->rxfull_event, &el);
}




/*
 * Transmitter thread.
 */
static THD_WORKING_AREA(can_tx_wa, 512);
static THD_FUNCTION(can_tx, p) {
  struct can_instance *cip = (struct can_instance*)p;
  CANTxFrame txmsg1;

  (void)p;
  chRegSetThreadName("transmitter");
  txmsg1.std.SID = 0x200;
  txmsg1.DLC = FDCAN_8BYTES_TO_DLC;
  txmsg1.common.XTD = 0;
  txmsg1.FDF = 0;         /* CAN frame format. */
  

  while (true) {
    for(size_t i=0; i< MOTORS_NB; i++) {
      txmsg1.data16[i] = __builtin_bswap16((int16_t)-cip->motors->at(i).get_cmd());
    }
    canTransmit(&CAND2, CAN_ANY_MAILBOX, &txmsg1, TIME_IMMEDIATE);
    chThdSleepMilliseconds(10);
  }
}

void motorsStart() {
  canStart(&CAND2, &cancfg);
  chThdCreateStatic(can_rx_wa, sizeof(can_rx_wa), NORMALPRIO + 7, can_rx, (void *)&can2);
  chThdCreateStatic(can_tx_wa, sizeof(can_tx_wa), NORMALPRIO + 7, can_tx, (void *)&can2);
}

void MotorCAN::set_cmd(double cmd)
{
   command = cmd * 100.0;
}

double MotorCAN::get_cmd()
{
   return command;
}

void MotorCAN::set_status(int16_t new_angle, int16_t new_speed, int16_t new_torque)
{
  pos += new_angle - angle;
  if(angle > 5460 && new_angle < 2730) {
      pos += 8192;
  } else if(angle < 2730 && new_angle > 5460) {
      pos -= 8192;
  }

  angle = new_angle;
  speed = new_speed;
  float alpha = 0.01;
  filter_speed = alpha*new_speed + (1-alpha)*filter_speed;
  torque = new_torque;
}

#endif