#include "guidanceHolo.h"
#include "globalVar.h"
#include "odometry.h"
#include <math.h>
#include "messages.h"
#include "mot_conf.h"
#include "Position.h" 

namespace e = enac;

constexpr double VMAX = 200; //   mm/s

constexpr double DIST_ACCURACY = 5;     //   mm
constexpr double THETA_ACCURACY = 1*DEG_TO_RAD; //   rad

void GuidanceHolo::init(){
}

void GuidanceHolo::set_target([[maybe_unused]] Position pos){
    // TODO
}


void GuidanceHolo::update() {
    // TODO
}
