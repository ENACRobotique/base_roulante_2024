#include "ch.h"
#include "hal.h"
#include "communication.h"
#include "telemetry.h"
#include "messages.h"
#include "odometry.h"
#include "globalVar.h"
#include "ins.h"

using namespace protoduck;

void send_pos(Message& msg);
void send_speed(Message& msg);

static THD_WORKING_AREA(telem, 1024);
 static void telemetry(void *) {
   chRegSetThreadName("telemetry");

   while (true) {
     Message msg;
     send_pos(msg);
     send_speed(msg);
     send_ins_report(msg);
     chThdSleepMilliseconds(100);
   }
 }


void telemetryStart(){

  chThdCreateStatic(telem, sizeof(telem), NORMALPRIO-1, telemetry, NULL);
}


void send_pos(Message& msg) {
  msg.clear();
  auto& pos = msg.mutable_pos();
  pos.set_x(odometry.get_x());
  pos.set_y(odometry.get_y());
  pos.set_theta(odometry.get_theta());
  pos.set_obj(Pos::PosObject::POS_ROBOT_W);

  post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);
}

void send_speed(Message& msg) {
  msg.clear();
  auto& pos = msg.mutable_speed();
  auto speed = odometry.get_speed();
  pos.set_vx(speed[0]);
  pos.set_vy(speed[1]);
  pos.set_vtheta(speed[2]);

  post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);
}
