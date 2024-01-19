/*
 * Based on the work of Florian Sansou <florian.sansou@enac.fr>
 * distibuted under the GNU General Public License v2
 */

/** @file filters/high_gain_filter.h
 *  @brief Implementation of the high gain filter for rotary encoder
 */

#pragma once

#include <math.h>
#include <array>

class HighGainFilter {
public:

  /** Init all matrix and vectors to the right value
   *
   * @param filter pointer to a filter structure
   * @param alpha 
   * @param epsilon high gain
   * @param rate data update rate
   * @param init_val_theta
   * @param init_val_theta_dot
   */
  void init(std::array<double, 3> alpha, double epsilon, double rate);
  void process(double theta);

  void reset(std::array<double, 3> hatx={0, 0, 0},
             std::array<double, 3> hatx_dot_prev={0, 0, 0}){
  this->hatx = hatx;
  this->hatx_dot_prev = hatx_dot_prev;
}

  void update_alpha0(double alpha0){ alpha[0] = alpha0; }
  void update_alpha1(double alpha1){ alpha[1] = alpha1; }
  void update_alpha2(double alpha2){ alpha[2] = alpha2; }
  void update_epsilon(double epsilon){ this->epsilon = epsilon; }
  void update_rate(double rate){ this->rate = rate; }

  double get_pos(){ return hatx[0];};
  double get_speed(){ return hatx[1];};


private:
  //states
  std::array<double, 3> hatx;  // state 
  std::array<double, 3> hatx_dot_prev; // previous state

  //parameters
  std::array<double, 3> alpha;
  float epsilon;                     //
  float rate;                     ///< data update rate (in Hz)

};
