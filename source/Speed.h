#pragma once
#include "messages.h"
#include <Eigen/LU>
#include "mot_conf.h"


class Speed {
public:
    Speed() {}

    Speed(float vx, float vy, float vtheta) : _vx(vx), _vy(vy), _vtheta(vtheta) {}

    Speed(enac::Speed speed) {
        _vx = speed.vx();
        _vy = speed.vy();
        _vtheta = speed.vtheta();
    }

    Speed(Eigen::Vector3d speed) {
        _vx = speed[0];
        _vy = speed[1];
        _vtheta = speed[2];
    }

    enac::Speed to_proto() {
        enac::Speed speed;
        speed.set_vx(_vx);
        speed.set_vy(_vy);
        speed.set_vtheta(_vtheta);
        return speed;
    }

    Eigen::Vector3d to_eigen() {
        return Eigen::Vector3d {_vx, _vy, _vtheta};
    }

    inline float vx() {return _vx;}
    inline float vy() {return _vy;}
    inline float vtheta() {return _vtheta;}

    inline void set_vx(float x) {_vx = x;}
    inline void set_vy(float y) {_vy = y;}
    inline void set_vtheta(float theta) {_vtheta = theta;}

    //inline double norm() {sqrt(pow(_vx,2) + pow(_vy,2));}
    
    Speed operator+(const Speed& other) const {
        enac::Speed result;
        result.set_vx(_vx + other._vx);
        result.set_vy(_vy + other._vy);
        result.set_vtheta(_vtheta + other._vtheta);
        return Speed(result);
    }

    Speed& operator+=(const Speed& other) {
        _vx = _vx + other._vx;
        _vy = _vy + other._vy;
        _vtheta = _vtheta + other._vtheta;
        return *this;
    }

    Speed operator-(const Speed& other) const {
        enac::Speed result;
        result.set_vx(_vx - other._vx);
        result.set_vy(_vy - other._vy);
        result.set_vtheta(_vtheta - other._vtheta);
        return Speed(result);
    }

    Speed& operator-=(const Speed& other) {
        _vx = _vx - other._vx;
        _vy = _vy - other._vy;
        _vtheta = _vtheta - other._vtheta;
        return *this;
    }

    Speed operator*(const float scale) const {
        enac::Speed result;
        result.set_vx(_vx * scale);
        result.set_vy(_vy * scale);
        result.set_vtheta(_vtheta * scale);
        return Speed(result);
    }

private:
    float _vx;
    float _vy;
    float _vtheta;
};
