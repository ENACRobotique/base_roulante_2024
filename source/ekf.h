#pragma once

#include <Eigen/Core>
#include "utils.h"
#include "ch.h"
#include "Position.h"

constexpr float GYRO_VAR = DEG_TO_RAD * 0.003 * 0.003;
constexpr float LIDAR_VAR_XY = 5*5;
constexpr float LIDAR_VAR_THETA = 0.004 * 0.004;
constexpr float ENC_VAR_V = 2*2;
constexpr float ENC_VAR_OMEGA = (DEG_TO_RAD*2) * (DEG_TO_RAD*2);
static Eigen::Matrix<float, 5, 1> MODEL_VAR {{5*5, 5*5, 0.1*0.1, 100*100, 100*100}};

class EKF
{
public:
    void init(float dt);

    void update_lidar(float xl, float yl, float thetal);
    void update_gyro(float omega_g);
    void update_encoders(float v_e, float omega_e);

    void predict();

    Position get_pos() {
        return Position(X(0), X(1), X(2));
    }

private:
    /*
    Model. Returns what should be the derivative of X, given X and U (command vector).
    f(X, U) = [V*cos(theta), V*sin(theta), omega, 0, 0]
    */
    float dt;

    mutex_t mtx;

    Eigen::Matrix<float, 5, 1> f();

    
    Eigen::Matrix<float, 5, 5> F();


    Eigen::Matrix<float, 5, 1> X;
    Eigen::Matrix<float, 5, 5> Q;
    Eigen::Matrix<float, 5, 5> P;

    static constexpr Eigen::Matrix<float, 3, 5> Hl {
        {1,0,0,0,0},
        {0,1,0,0,0},
        {0,0,1,0,0}
    };
    
    static constexpr Eigen::Matrix<float, 1, 5> Hg {{0,0,0,0,1}};

    static constexpr Eigen::Matrix<float, 2, 5> He {
        {0,0,0,1,0},
        {0,0,0,0,1}
    };

    

    static constexpr Eigen::Matrix<float, 3, 3> Rl {
        {LIDAR_VAR_XY, 0, 0},
        {0, LIDAR_VAR_XY, 0},
        {0, 0, LIDAR_VAR_THETA}
    };

    Eigen::Matrix<float, 1, 1> Rg;
    Eigen::Matrix<float, 2, 2> Re;

    


};