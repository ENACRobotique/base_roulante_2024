#pragma once
#define DRIVE_DIFF 0
#define DRIVE_HOLO 1

// config

constexpr double ROBOT_RADIUS = 128.77333333333334;
constexpr uint8_t MOTORS_NB = 2;
#define DRIVE DRIVE_DIFF


// DO NOT EDIT BELOW THIS LINE

#if !defined(DRIVE) || (DRIVE != DRIVE_DIFF && DRIVE != DRIVE_HOLO)
#error DRIVE must be defined to either DRIVE_DIFF or DRIVE_HOLO
#endif



#if DRIVE == DRIVE_DIFF
static_assert(MOTORS_NB == 2, "A differential robot should have 2 motors");
#elif DRIVE == DRIVE_HOLO
#include <Eigen/Core>

static_assert(MOTORS_NB == 3 || MOTORS_NB == 4, "An holonomic robot should have 3 or 4 motors");

using VectMot = Eigen::Matrix<double, MOTORS_NB, 1>;


template<int NB>
    requires (NB == 3 || NB == 4)
struct MotorConfig;


//       _____                    __                 
//      |__  /   ____ ___  ____  / /_____  __________
//       /_ <   / __ `__ \/ __ \/ __/ __ \/ ___/ ___/
//     ___/ /  / / / / / / /_/ / /_/ /_/ / /  (__  ) 
//    /____/  /_/ /_/ /_/\____/\__/\____/_/  /____/  
//                                                  
// motors angles : 0, 2*pi/3, -2*pi/3
// X axis is the front of the robot, Z up.
//
//         X
//
//         0 
//
//         .
//    2         1
//

template<> struct MotorConfig<3> {
  static constexpr Eigen::Matrix<double, 3, 3> D {
    {-0.0,                1.0,                  ROBOT_RADIUS},
    {-0.8660254037844387, -0.4999999999999998,  ROBOT_RADIUS},
    {0.8660254037844387,  -0.4999999999999998,  ROBOT_RADIUS},
  };

  static constexpr Eigen::Matrix<double, 3, 3> Dinv {
    {0.0,                 -0.5773502691896256,  0.5773502691896256},
    {0.666666666666667,   -0.3333333333333334,  -0.3333333333333333},
    {1/(3*ROBOT_RADIUS),  1/(3*ROBOT_RADIUS),   1/(3*ROBOT_RADIUS)},
  };
};


//       __ __                     __                 
//      / // /    ____ ___  ____  / /_____  __________
//     / // /_   / __ `__ \/ __ \/ __/ __ \/ ___/ ___/
//    /__  __/  / / / / / / /_/ / /_/ /_/ / /  (__  ) 
//      /_/    /_/ /_/ /_/\____/\__/\____/_/  /____/  
//                                                    
// motors angles : pi/4, 3*pi/4, -3*pi/4, -pi/4
// X axis is the front of the robot, Z up.
//
//        X
//
//     3     0
//        . 
//
//     2     1
//

template<> struct MotorConfig<4> {
  static constexpr Eigen::Matrix<double, 4, 3> D {
    {-0.7071067811865475,  0.7071067811865476, ROBOT_RADIUS},
    {-0.7071067811865476, -0.7071067811865475, ROBOT_RADIUS},
    {0.7071067811865476,  -0.7071067811865475, ROBOT_RADIUS},
    {0.7071067811865475,  0.7071067811865476,  ROBOT_RADIUS},
  };

  static constexpr Eigen::Matrix<double, 3, 4> Dinv {
    {-0.3535533905932738, -0.3535533905932738,  0.3535533905932738,   0.3535533905932738},
    {0.35355339059327395, -0.35355339059327395, -0.35355339059327384, 0.3535533905932738},
    {1/(4*ROBOT_RADIUS),  1/(4*ROBOT_RADIUS),   1/(4*ROBOT_RADIUS),   1/(4*ROBOT_RADIUS)},
  };
};


//----------------------------------------------------------//


static Eigen::Matrix<double, MOTORS_NB, 3> D = MotorConfig<MOTORS_NB>::D;
static Eigen::Matrix<double, 3, MOTORS_NB> Dinv = MotorConfig<MOTORS_NB>::Dinv;

#endif
