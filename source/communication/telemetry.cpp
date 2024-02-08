#include "ch.h"
#include "hal.h"
#include "communication.h"
#include "telemetry.h"
#include "messages.h"
#include "odometry.h"
#include "globalVar.h"

using namespace protoduck;

static THD_WORKING_AREA(telem, 1024);
 static void telemetry(void *) {
   chRegSetThreadName("telemetry");

   while (true) {

    
    Message msg;
    auto& pos = msg.mutable_pos();
    pos.set_x(odometry.get_x());
    pos.set_y(odometry.get_y());
    pos.set_theta(odometry.get_theta());
    pos.set_obj(Pos::PosObject::POS_ROBOT_W);

    post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);
    
     chThdSleepMilliseconds(200);
   }
 }


void telemetryStart(){

  chThdCreateStatic(telem, sizeof(telem), NORMALPRIO-1, telemetry, NULL);
}
