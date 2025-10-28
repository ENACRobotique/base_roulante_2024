#pragma once
#include <ch.h>
#include <Eigen/Core>
#include "Position.h" 


class Guidance{
    private:
    public:
        virtual void init() = 0;
        virtual void set_target(Position pos) = 0;
        virtual void update() = 0;
};