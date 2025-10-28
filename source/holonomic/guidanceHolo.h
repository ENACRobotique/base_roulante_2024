#pragma once
#include <ch.h>
#include <Eigen/Core>
#include "Position.h" 
#include "guidance.h"

class GuidanceHolo: public Guidance {
    private:
        enum GuidanceState{
            RUNNING,
            IDLE
        };
        Position start_pos;
        Position target_pos;
        systime_t start_time;
        GuidanceState state;
        double estimated_time;


    public:
        void init() override;
        void set_target(Position pos) override;
        void update() override;
};