#pragma once

#include <functional>
#include "messages.h"
#include <hal.h>
#include "mot_conf.h"
namespace e = enac;


#define NUM_MESSAGES 30
extern mailbox_t mb_free_msgs;
extern mailbox_t mb_filled_msgs;

enum RcvState {
    _RCV_START1ST,
    _RCV_START2ND,
    _RCV_LEN,
    _RCV_PAYLOAD,
    _RCV_CHK,
};

enum MessagesStates {
    COM_OK,
    COM_NO_MSG,
    COM_ERROR,
};

typedef std::function<void(e::Message<MOTORS_NB>&)> msg_callback_t;


void register_callback(msg_callback_t cb);
void communicationStart(void);
msg_t post_message(e::Message<MOTORS_NB>& msg, e::Message<MOTORS_NB>::MsgType msg_type, sysinterval_t timeout);
