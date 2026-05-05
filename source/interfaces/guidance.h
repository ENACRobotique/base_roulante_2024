#pragma once
#include <ch.h>
#include <Eigen/Core>
#include "Position.h" 
#include <optional>


class Guidance{
    public:
    enum Referentiel{
        TABLE,
        ROBOT
    };
    public:
        virtual void init() = 0;
        virtual void set_target(Position pos, std::optional<double> direction, Referentiel ref = Referentiel::TABLE) = 0;
        virtual void update() = 0;
};
