#pragma once
#include "communication.h"

void insStart();
double ins_get_theta();
double ins_get_vtheta();
void ins_set_theta(double theta);

void send_ins_report(Message& msg);


