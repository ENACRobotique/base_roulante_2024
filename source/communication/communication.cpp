
#include <ch.h>
#include <hal.h>
#include "printf.h"
#include "stdutil.h"
#include "globalVar.h"
#include "communication.h"
#include "BytesReadBuffer.h"
#include "BytesWriteBuffer.h"

#define MAX_SIZE 50

uint8_t Timeout_rx = chTimeMS2I(20);

uint8_t full_message[MAX_SIZE+4];

BytesWriteBuffer msgBuffer[NUM_MESSAGES];

msg_t free_messages_queue[NUM_MESSAGES];
MAILBOX_DECL(mb_free_msgs, free_messages_queue, NUM_MESSAGES);

msg_t filled_messages_queue[NUM_MESSAGES];
MAILBOX_DECL(mb_filled_msgs, filled_messages_queue, NUM_MESSAGES);


constexpr size_t NUM_CALLBACKS = 10;
msg_callback_t callbacks[NUM_CALLBACKS] = {0};


static const SerialConfig serialConfig = {
  .speed = 230400,
  .cr1 = 0,
  .cr2 = USART_CR2_STOP1_BITS | USART_CR2_LINEN,
  .cr3 = 0
};

SerialDriver* Serial = &SD4;


static int check_messages(Message& dmsg, BytesReadBuffer& read_buffer);
static void com_rx (void *);
static void com_tx (void *);


static THD_WORKING_AREA(waComRx, 2048);
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
  }
}


// send throught UART all messages ready to be sent.
static void com_tx (void *) {
  chRegSetThreadName("com_tx");
  BytesWriteBuffer *buffer;
  while (true) {
    // get a filled buffer.
    chMBFetchTimeout(&mb_filled_msgs, (msg_t *)&buffer, TIME_INFINITE);
    uint32_t buf_size = buffer->get_size();
    uint8_t* data = buffer->get_data();

    if (buf_size > 255){
      buffer->clear();
      (void)chMBPostTimeout(&mb_free_msgs, (msg_t)buffer, TIME_IMMEDIATE);
      continue;
    }

    uint8_t chk = 0;
    for(size_t i=0; i<buf_size; i++) {
      chk ^= data[i];
    }

    
    full_message[0] = 0xFF;
    full_message[1] = 0xFF;
    full_message[2] = buf_size;
    for (size_t i=0; i<buf_size; i++){
      full_message[i+3] = data[i];
    }
    full_message[buf_size +3] = chk;

    sdWrite(&SD4, full_message, buf_size +4);

    buffer->clear();
    // return buffer to the free buffers pool.
    // can't fail right ? Just fetched a message, and filled and free have the same size
    (void)chMBPostTimeout(&mb_free_msgs, (msg_t)buffer, TIME_IMMEDIATE);
  }
}


/**
 *  Received message from serial.
 *  Returns COM_OK if a message is available.
 */
static int check_messages(Message& dmsg, BytesReadBuffer& read_buffer) {
  static uint8_t rx_buf[50] = {0};
  rx_buf[0] = rx_buf[1] = 0;

  while((rx_buf[0] != 0xFF) || (rx_buf[1] =! 0xFF)) {
    rx_buf[0] = rx_buf[1];
    sdReadTimeout(&SD4, rx_buf, 1, TIME_INFINITE);
  }
  
  if (sdReadTimeout(&SD4, &rx_buf[1], 1, Timeout_rx) != 1) {
    return COM_NO_MSG;
  }

  if (sdReadTimeout(&SD4, &rx_buf[2], 1, Timeout_rx) != 1) {
    return COM_NO_MSG; // length byte
  }

  uint32_t msg_length = rx_buf[2];
  if(msg_length == 0) {
    return COM_ERROR;
    }

  if (sdReadTimeout(&SD4, &rx_buf[3], msg_length, Timeout_rx) != msg_length){
    return COM_NO_MSG; // message bytes
  }

  read_buffer.clear();
  read_buffer.push(&rx_buf[3], msg_length);
  uint8_t check_sum = 0;
  for (size_t i=0; i <msg_length; i++){
    check_sum ^= rx_buf[i+3];
  }
  uint8_t chk_rcv;
  if (sdReadTimeout(&SD4, &chk_rcv, 1, Timeout_rx) != 1) {
    return COM_NO_MSG; // check_sum byte
  }
  if (check_sum == chk_rcv){
    dmsg.clear();
    dmsg.deserialize(read_buffer);
    return COM_OK;
  } else {
    return COM_ERROR;
  }


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
