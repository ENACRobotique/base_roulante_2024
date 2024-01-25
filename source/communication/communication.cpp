
#include <ch.h>
#include <hal.h>
#include "printf.h"
#include "stdutil.h"
#include "globalVar.h"
#include "communication.h"
#include "BytesReadBuffer.h"
#include "BytesWriteBuffer.h"

#define MAX_SIZE 50


BytesWriteBuffer msgBuffer[NUM_MESSAGES];

msg_t free_messages_queue[NUM_MESSAGES];
MAILBOX_DECL(mb_free_msgs, free_messages_queue, NUM_MESSAGES);

msg_t filled_messages_queue[NUM_MESSAGES];
MAILBOX_DECL(mb_filled_msgs, free_messages_queue, NUM_MESSAGES);


constexpr size_t NUM_CALLBACKS = 10;
msg_callback_t callbacks[NUM_CALLBACKS] = {0};


static const SerialConfig serialConfig = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = USART_CR2_STOP1_BITS | USART_CR2_LINEN,
  .cr3 = 0
};

SerialDriver* Serial = &SD4;


static int check_messages(Message& dmsg, BytesReadBuffer& read_buffer);
static void com_rx (void *);
static void com_tx (void *);


void comm_init() {
    sdStart(&SD4, &serialConfig);
}

static THD_WORKING_AREA(waComRx, 1024);
static THD_WORKING_AREA(waComTx, 1024);
void communicationStart() {
  sdStart(&SD4, &serialConfig);
  
  // Pre-filling the free buffers pool with the available buffers, the post
  // will not stop because the mailbox is large enough.
  for (int i = 0; i < NUM_MESSAGES; i++) {
    (void)chMBPostTimeout(&mb_free_msgs, (msg_t)&msgBuffer[i], 0);
  }

  chThdCreateStatic(waComRx, sizeof(waComRx), NORMALPRIO, &com_rx, NULL);
  chThdCreateStatic(waComTx, sizeof(waComTx), NORMALPRIO, &com_tx, NULL);
}



void register_callback(msg_callback_t cb) {
    for(size_t i=0; i<NUM_CALLBACKS; i++) {
        if(!callbacks[i]) {
            callbacks[i] = cb;
            break;
        }
    }
}




static void com_rx (void *)
{
  chRegSetThreadName("com_rx");

  BytesReadBuffer read_buffer;

  Message msg;

  while (true) {
    int ret = check_messages(msg, read_buffer);
    if(ret == COM_OK) {
      for(size_t i=0; i<NUM_CALLBACKS; i++) {
          if(callbacks[i]) {
              callbacks[i](msg);
          }
      }
    }
    chThdSleepMilliseconds(1);
  }
}


static void com_tx (void *) {
  chRegSetThreadName("com_tx");
  BytesWriteBuffer *buffer;
  while (true) {    // the while(true) is probably useless...
    // send throught UART all messages ready to be sent.
    // get a filled buffer.
    while(chMBFetchTimeout(&mb_filled_msgs, (msg_t *)&buffer, TIME_INFINITE) == MSG_OK) {
        uint32_t buf_size = buffer->get_size();
        uint8_t* data = buffer->get_data();

        uint8_t chk = 0;
        for(size_t i=0; i<buf_size; i++) {
            chk ^= data[i];
        }

        /**
         * 
         * 
         * 
         * 
         * 
         * envoie la trame : 0XFF, 0XFF, LEN, PAYLOAD, CHK
         * 
         * 
         * 
         * 
         * 
         * 
        */

        buffer->clear();
        // return buffer to the free buffers pool.
        // can't fail right ? Just fetched a message, and filled and free have the same size
        (void)chMBPostTimeout(&mb_free_msgs, (msg_t)buffer, 0);
    }
  }
}


/**
 *  Received message from serial.
 *  Returns COM_OK if a message is available.
 */
static int check_messages(Message& dmsg, BytesReadBuffer& read_buffer) {
    dmsg.clear();
    static enum RcvState _rcv_state = _RCV_START1ST;

    /**
     * 
     * 
     * 
     *  décode la trame : 0XFF, 0XFF, LEN, PAYLOAD, CHK
     * (et vérifie la checksum)
     * 
     * 
     * 
    */

    return COM_NO_MSG;
}


msg_t post_message(Message& msg, Message::MsgType msg_type, sysinterval_t timeout) {
  BytesWriteBuffer* buffer;
  // get a free buffer
  msg_t ret = chMBFetchTimeout(&mb_free_msgs, (msg_t *)&buffer, timeout);
  if(ret != MSG_OK) {
    return ret;
  }

  msg.set_msg_type(msg_type);
  msg.serialize(*buffer);
  // post the new message for the communication thread. no timeout.
  return chMBPostTimeout(&mb_filled_msgs, (msg_t)buffer, TIME_IMMEDIATE);
}
