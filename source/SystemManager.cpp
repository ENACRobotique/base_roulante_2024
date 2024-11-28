#include "SystemManager.h"
#include "motor.h"

void SystemManager::stop_motors(){
    set_motor(0, 0);
    set_motor(1, 0);
    set_motor(2, 0);
}
