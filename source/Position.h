#pragma once
#include "messages.h"
#include <Eigen/LU>
#include "mot_conf.h"
#include "utils.h"

class Position {
public:
    Position() {}

    Position(float x, float y, float theta) : _x(x), _y(y), _theta(theta) {}

    Position(enac::Position pos) {
        _x = pos.x();
        _y = pos.y();
        _theta = pos.theta();
    }

    Position(Eigen::Vector3d pos) {
        _x = pos[0];
        _y = pos[1];
        _theta = pos[2];
    }

    enac::Position to_proto() {
        enac::Position pos;
        pos.set_x(_x);
        pos.set_y(_y);
        pos.set_theta(_theta);
        return pos;
    }

    Eigen::Vector3d to_eigen() {
        return Eigen::Vector3d {_x, _y, _theta};
    }

    inline float x() {return _x;}
    inline float y() {return _y;}
    inline float theta() {return _theta;}

    inline void set_x(float x) {_x = x;}
    inline void set_y(float y) {_y = y;}
    inline void set_theta(float theta) {_theta = theta;}
    void center_theta() {
        while (_theta > M_PI){
            _theta -= 2*M_PI;
        }
        while (_theta < -M_PI){
            _theta += 2*M_PI;
        }
    }

    inline double distance(Position pos2) {return sqrt(pow(_x-pos2.x(), 2) + pow(_y-pos2.y(), 2));}

    /**
     * Exprime le point dans le repère @param frame.
     * @param this et @param frame doivent être exprimés dans le même repère (par exemple la table)
     */
    Position to_frame(Position frame){
        // matrice de rotation: https://fr.wikipedia.org/wiki/Matrice_de_rotation#En_deux_dimensions
        //
        // | xf | = | cos(theta)   sin(theta)    0 | . | x0 |
        // | yf |   | -sin(theta)  cos(theta)    0 |   | y0 |

        double c = cos(frame.theta());
        double s = sin(frame.theta());

        double dx = _x - frame.x();
        double dy = _y - frame.y();

        double xf = dx * c + dy * s;
        double yf = dy * c - dx * s;
        Position result = Position(xf,yf,_theta-frame.theta());
        result.center_theta();
        return result;
    }

    double gisement(){
        return atan2(_y,_x);
    }

    Position operator+(const Position& other) const {
        enac::Position result;
        result.set_x(_x + other._x);
        result.set_y(_y + other._y);
        result.set_theta(_theta + other._theta);
        return Position(result);
    }

    Position& operator+=(const Position& other) {
        _x = _x + other._x;
        _y = _y + other._y;
        _theta = _theta + other._theta;
        return *this;
    }

    Position operator-(const Position& other) const {
        enac::Position result;
        result.set_x(_x - other._x);
        result.set_y(_y - other._y);
        result.set_theta(_theta - other._theta);
        return Position(result);
    }

    Position& operator-=(const Position& other) {
        _x = _x - other._x;
        _y = _y - other._y;
        _theta = _theta - other._theta;
        return *this;
    }

    Position operator*(double other) const {
        return Position(_x * other, _y * other, _theta * other);
    }

private:
    float _x;
    float _y;
    float _theta;
};

inline Position operator*(double lhs, const Position& rhs) {
    return rhs * lhs;
}
