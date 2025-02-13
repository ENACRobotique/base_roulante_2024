#include "SystemManager.h"
#include "motor.h"
#include "globalVar.h"

void SystemManager::stop_motors(){
    mot1.set_cmd(0);
    mot2.set_cmd(0);
    mot3.set_cmd(0);
}
