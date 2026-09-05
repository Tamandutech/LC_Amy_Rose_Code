/*
 * PID.cpp
 *
 *  Created on: Sep 2, 2026
 *      Author: felipe
 */

#include "PID.hpp"
#include <cstdio>
#include <cstring>

#include "../../Context/GlobalData.hpp"
#include "../../peripherals.h"

float pidLastError = 0;

volatile float dd = 0;
volatile float dp = 0;

float pidEvaluate(const int error) {
  float P   = error;
  float D   = error - pidLastError;
  float pid = ((kp + dp) * P) + ((kd + dd) * D);

  char tx_buffer[128];

  snprintf(tx_buffer, sizeof(tx_buffer),
           "pid: %f | error: %d | pwmL: %f | pwmR: %f\n", pid, error,
           MOTOR_PWM + pid, MOTOR_PWM - pid);

  HAL_UART_Transmit(&BLE_BUS, (uint8_t *)tx_buffer, strlen(tx_buffer), 25);

  pidLastError = error;

  return pid;
}
