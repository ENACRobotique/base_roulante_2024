#pragma once
#include "communication.h"

void insStart();
double ins_get_theta();
double ins_get_vtheta();

void send_ins_report(Message& msg);
