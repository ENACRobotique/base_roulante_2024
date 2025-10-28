#pragma once
#include <ch.h>
#include <Eigen/Core>
#include "Position.h" 
#include "guidance.h"


class GuidanceDiff : public Guidance {
    private:
        enum GuidanceState{
            INITIAL_TURN_TO_TARGET,
            AVANCING_TO_TARGET,
            FINAL_TURN,
            IDLE
        };
        Position start_pos;
        Position target_pos;
        systime_t last_time;
        GuidanceState state;
        double estimated_time;

    public:
        void init() override;
        void set_target(Position pos) override;
        void update() override;
};
