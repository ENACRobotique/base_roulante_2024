#pragma once
#include "messages.h"
#include <Eigen/LU>
#include "mot_conf.h"


class Position {
public:
    Position() {}

    Position(enac::Position pos): _pos(pos) {}

    Position(Eigen::Vector3d pos) {
        _pos.set_x(pos[0]);
        _pos.set_y(pos[1]);
        _pos.set_theta(pos[2]);
    }

    Position(float x, float y, float theta) {
        _pos.set_x(x);
        _pos.set_y(y);
        _pos.set_theta(theta);
    }

    enac::Position& to_proto() {return _pos;}

    Eigen::Vector3d to_eigen() {
        return Eigen::Vector3d {_pos.get_x(), _pos.get_y(), _pos.get_theta()};
    }

    inline float x() {return _pos.x();}
    inline float y() {return _pos.y();}
    inline float theta() {return _pos.theta();}

    inline void set_x(float x) {_pos.set_x(x);}
    inline void set_y(float y) {_pos.set_y(y);}
    inline void set_theta(float theta) {_pos.set_theta(theta);}

    
    Position operator+(const Position& other) const {
        enac::Position result;
        result.set_x(_pos.get_x() + other._pos.get_x());
        result.set_y(_pos.get_y() + other._pos.get_y());
        result.set_theta(_pos.get_theta() + other._pos.get_theta());
        return Position(result);
    }

    Position& operator+=(const Position& other) {
        _pos.set_x(_pos.get_x() + other._pos.get_x());
        _pos.set_y(_pos.get_y() + other._pos.get_y());
        _pos.set_theta(_pos.get_theta() + other._pos.get_theta());
        return *this;
    }

    Position operator-(const Position& other) const {
        enac::Position result;
        result.set_x(_pos.get_x() - other._pos.get_x());
        result.set_y(_pos.get_y() - other._pos.get_y());
        result.set_theta(_pos.get_theta() - other._pos.get_theta());
        return Position(result);
    }

    Position& operator-=(const Position& other) {
        _pos.set_x(_pos.get_x() - other._pos.get_x());
        _pos.set_y(_pos.get_y() - other._pos.get_y());
        _pos.set_theta(_pos.get_theta() - other._pos.get_theta());
        return *this;
    }

private:
    enac::Position _pos;
};
