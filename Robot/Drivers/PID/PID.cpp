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

float pidLastError = 0;

float kp = 0.35;
float kd = 2;

float pidEvaluate(const int error) {
  float P   = error;
  float D   = error - pidLastError;
  float pid = ((kp)*P) + ((kd)*D);
  /*
  char tx_buffer[128];

  snprintf(tx_buffer, sizeof(tx_buffer),
           "pid: %d | error: %d | pwmL: %d | pwmR: %d\n", (int)pid, error,
           MOTOR_PWM + (int)pid, MOTOR_PWM - (int)pid);

  HAL_UART_Transmit(&BLE_BUS, (uint8_t *)tx_buffer, strlen(tx_buffer), 25);
  */

  pidLastError = error;

  return pid;
}
