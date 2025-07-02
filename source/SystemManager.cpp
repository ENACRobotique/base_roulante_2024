#include "SystemManager.h"
#include "motor.h"
#include "globalVar.h"

void SystemManager::stop_motors(){
    for(size_t i=0; i<MOTORS_NB; i++) {
        motors[i].set_cmd(0);
    }
}
