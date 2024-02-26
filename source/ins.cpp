#include "ins.h"
#include "lsm6dsl.h"
#include "ch.h"
#include "communication.h"
#include "RunningStat.h"

RunningStat stat;

#define NB_ALIGN_SAMPLES 2000

constexpr double scale_factor = 1.1842105263157894;
constexpr double rate = 833.0;

size_t align = NB_ALIGN_SAMPLES;


double ins_vtheta = 0;
double ins_theta = 0;
double bias = 0;


static THD_WORKING_AREA(waINS, 512);
static void ins(void *) {

    chThdSleepMilliseconds(3000);

    while(true) {
        while((imu_data_read_index+1)%IMU_DATA_NB != imu_data_write_index) {

            imu_data_t data = imu_data_buffer[imu_data_read_index];
            imu_data_read_index = (imu_data_read_index+1)%IMU_DATA_NB;

            if(align > 0) {
                align -= 1;
                bias += (double)data.gz / NB_ALIGN_SAMPLES;
                stat.Push(data.gz);
                if(align == 1) {
                    ins_theta = 0;
                }
            } else {
                ins_vtheta = ((double)data.gz - bias) * scale_factor;
                ins_theta += ins_vtheta / rate;
                // ins_theta += ((double)data.gz - bias) / 833.0 * scale_factor;
            }
        }

        chThdSleepMilliseconds(5);
    }
}


void insStart() {
    chThdCreateStatic(waINS, sizeof(waINS), NORMALPRIO, ins, NULL);
}


double ins_get_theta() {
    return ins_theta;
}

double ins_get_vtheta() {
    return ins_vtheta;
}

void ins_set_theta(double theta) {
    ins_theta = theta;
}

void send_ins_report(Message& msg) {
    msg.clear();
    auto& ins = msg.mutable_ins();
    ins.set_vtheta((float)ins_vtheta);
    ins.set_theta((float)ins_theta);
    ins.set_bias((float)bias);
    ins.set_variance(stat.Variance());
    post_message(msg, protoduck::Message::MsgType::STATUS, TIME_IMMEDIATE);
}
