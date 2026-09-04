/*
 * PID.cpp
 *
 *  Created on: Sep 2, 2026
 *      Author: felipe
 */

#include "PID.hpp"

float pidLastError = 0;

float pidEvaluate(const int error) {
  float P   = error;
  float D   = error - pidLastError;
  float pid = (kp * P) + (kd * D);

  pidLastError = error;

  return pid;
}
