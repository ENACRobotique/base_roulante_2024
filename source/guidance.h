#pragma once
#include <ch.h>
#include <Eigen/Core>


class Guidance{
    private:
        enum GuidanceState{
            RUNNING,
            IDLE
        };
        Eigen::Vector3d start_pos;
        Eigen::Vector3d target_pos;
        systime_t start_time;
        GuidanceState state;
        double estimated_time;


    public:
        void init();
        void set_target(Eigen::Vector3d pos);
        void update();
};