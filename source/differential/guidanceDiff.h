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
            IDLE,
            COURBE,
            PURE_PURSUIT
        };

        enum class MvtDirection : int32_t{
            FORWARD=1,
            BACKWARD=-1,
            UNDEFINED=0
        };

        Position start_pos;
        Position target_pos;
        systime_t last_time;
        GuidanceState state;
        MvtDirection mvt_direction;
        double estimated_time;

    public:
        void init() override;
        void set_target(Position pos, std::optional<double> direction = std::nullopt);
        void update() override;
};
