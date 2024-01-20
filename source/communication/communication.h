#pragma once

#include <functional>
#include "messages.h"
#include <hal.h>

using namespace protoduck;

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_MESSAGES 10
extern mailbox_t mb_free_msgs;
extern mailbox_t mb_filled_msgs;

enum RcvState {
    _RCV_START1ST,
    _RCV_START2ND,
    //_RCV_ID,
    _RCV_LEN,
    _RCV_PAYLOAD,
    _RCV_CHK,
};

enum MessagesStates {
    COM_OK,
    COM_NO_MSG,
    COM_ERROR,
};

typedef std::function<void(Message&)> msg_callback_t;


void register_callback(msg_callback_t cb);
void start_communication(void);
msg_t post_message(protoduck::Message& msg, protoduck::Message::MsgType msg_type, sysinterval_t timeout);

#ifdef __cplusplus
}
#endif
