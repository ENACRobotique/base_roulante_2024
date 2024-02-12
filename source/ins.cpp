#include "ins.h"
#include "lsm6dsl.h"
#include "ch.h"
#include "communication.h"
#include "RunningStat.h"

RunningStat stat;

#define NB_ALIGN_SAMPLES 2000

size_t align = NB_ALIGN_SAMPLES;

static THD_WORKING_AREA(waINS, 512);
static void ins(void *) {

    chThdSleepMilliseconds(3000);

    double theta = 0;
    double bias = 0;

    while(true) {
        while((imu_data_read_index+1)%IMU_DATA_NB != imu_data_write_index) {

            imu_data_t data = imu_data_buffer[imu_data_read_index];
            imu_data_read_index = (imu_data_read_index+1)%IMU_DATA_NB;

            if(align > 0) {
                align -= 1;
                bias += (double)data.gz / NB_ALIGN_SAMPLES;
                stat.Push(data.gz);
                if(align == 1) {
                    theta = 0;
                }
            } else {
                theta += ((double)data.gz - bias) / 833.0;
            }
        }


        float theta_out = theta / 304.0 * 360.0;
        Message msg;
        auto& ins = msg.mutable_ins();
        ins.set_theta((float)theta_out);
        ins.set_bias((float)bias);
        ins.set_variance(stat.Variance());
        post_message(msg, protoduck::Message::MsgType::STATUS, TIME_IMMEDIATE);

        chThdSleepMilliseconds(5);
    }
}


void insStart() {
    chThdCreateStatic(waINS, sizeof(waINS), NORMALPRIO, ins, NULL);
}
