#include "ekf.h"
#include "math.h"
#include <Eigen/LU>

void EKF::init(float dt)
{
    this->dt = dt;
    Rg << GYRO_VAR;
    Re <<
        ENC_VAR_V, 0,
        0, ENC_VAR_OMEGA;
    
    Eigen::Matrix<float, 5, 5> noise_mat {
        {0.25f*powf(dt,4), 0.5f*powf(dt,3),        0,         0,        0},
        { 0.5f*powf(dt,3),     powf(dt,2),         0,         0,        0},
        {              0,              0,         dt,         0,        0},
        {              0,              0,          0,        dt,        0},
        {              0,              0,          0,         0,       dt}
    };
    
    Q = noise_mat * MODEL_VAR.asDiagonal();
    P = Eigen::Matrix<float, 5, 5>::Identity();
    chMtxObjectInit(&mtx);
}

void EKF::update_lidar(float xl, float yl, float thetal)
{
    chMtxLock(&mtx);
    float x = X(0);
    float y = X(1);
    float theta = X(2);
    Eigen::Matrix<float, 3, 1> Y {xl - x, yl - y, static_cast<float>(center_radians(thetal - theta))};
    Eigen::Matrix<float, 5, 3> K = P * Hl.transpose() * (Hl * P*Hl.transpose() + Rl).inverse();
    X = X + K * Y;
    P = (Eigen::Matrix<float, 5, 5>::Identity() - K * Hl) * P;
    chMtxUnlock(&mtx);
}

void EKF::update_gyro(float omega_g)
{
    chMtxLock(&mtx);
    float omega = X(4);
    float Y = omega_g - omega;
    Eigen::Matrix<float, 5, 1> K = P * Hg.transpose() * (Hg * P*Hg.transpose() + Rg).inverse();
    X = X + K * Y;
    P = (Eigen::Matrix<float, 5, 5>::Identity() - K * Hg) * P;
    chMtxUnlock(&mtx);
}

void EKF::update_encoders(float v_e, float omega_e)
{
    chMtxLock(&mtx);
    float v = X(3);
    float omega = X(4);
    
    Eigen::Matrix<float, 2, 1> Y {v_e - v, omega_e - omega};
    Eigen::Matrix<float, 5, 2> K = P * He.transpose() * (He * P*He.transpose() + Re).inverse();
    X = X + K * Y;
    P = (Eigen::Matrix<float, 5, 5>::Identity() - K * He) * P;
    chMtxUnlock(&mtx);
}

void EKF::predict()
{
    chMtxLock(&mtx);
    X = X + f()*dt;
    Eigen::Matrix<float, 5, 5> jac = Eigen::Matrix<float, 5, 5>::Identity() + F()*dt;
    P = jac * P * jac.transpose() + Q;
    chMtxUnlock(&mtx);
}

Eigen::Matrix<float, 5, 1> EKF::f()
{
    float theta = X(2);
    float v = X(3);
    float omega = X(4);

    Eigen::Matrix<float, 5, 1> Xdot {
        v * cos(theta),
        v *sin(theta),
        omega,
        0,
        0
    };
    return Xdot;
}

Eigen::Matrix<float, 5, 5> EKF::F(){
    float theta = X(2);
    float v = X(3);
    float c = cos(theta);
    float s = sin(theta);

    Eigen::Matrix<float, 5, 5> Jacob {
        {0,0, -v*s,c,0},
        {0,0,v*c,s,0},
        {0, 0, 0,    0, 1},
        {0, 0, 0,    0, 0},
        {0, 0, 0,    0, 0}
    };
    return Jacob;
}

