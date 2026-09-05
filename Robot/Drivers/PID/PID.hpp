/*
 * PID.hpp
 *
 *  Created on: Sep 2, 2026
 *      Author: felipe
 */

#ifndef DRIVERS_PID_PID_HPP_
#define DRIVERS_PID_PID_HPP_

#define kp 0
#define kd 0

extern volatile float dd;
extern volatile float dp;

float pidEvaluate(const int error);


#endif /* DRIVERS_PID_PID_HPP_ */
