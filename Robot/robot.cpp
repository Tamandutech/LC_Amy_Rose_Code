/*
 * robot.cpp
 *
 *  Created on: Aug 27, 2026
 *      Author: Kelvin Novais
 */

#include "robot.h"
#include "peripherals.h"

#include "Context/GlobalData.hpp"

#include "Drivers/BLE/ble.hpp"
#include "Drivers/Motors/Motors.hpp"
#include "Drivers/PID/PID.hpp"
#include "Drivers/Sensors/Sensors.hpp"
#include "Drivers/Vacuum/vacuum.hpp"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_uart.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <math.h>

void setup(void) {
  // Initialize base timer
  HAL_TIM_Base_Start(&BASE_TIMER);

  // Initialize ADC (needed for IRSensors and Battery)
  HAL_ADCEx_Calibration_Start(&ADC_1, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&ADC_2, ADC_SINGLE_ENDED);
  HAL_Delay(100);

  // Initialize DMA (needed for IRSensors and Battery)
  HAL_ADC_Start_DMA(&ADC_1, (uint32_t *)adc1_buffer, ADC_BUFFER_SIZE);
  HAL_ADC_Start_DMA(&ADC_2, (uint32_t *)adc2_buffer, ADC_BUFFER_SIZE);
  HAL_Delay(100);

  motorsInitialize();
  vacuumInitialize();

  char PID_Params[128];

  float x = 0, y = 0;

  HAL_UART_Receive(&BLE_BUS, (uint8_t *)PID_Params, 9, 100000);

  sscanf(PID_Params, "%f %f", &x, &y);

  dp = x - kp;

  dd = y - kd;

  HAL_Delay(5000);
  BLEInitialize();

  BLEMessagePush("RUN (1)\nSTOP (2)\nCALIBRATE (3)\nTEST SENSORS (4)");

  // ...
}

void run() {
  while(action == RUN) {
    sensorsUpdate();

    int u = pidEvaluate(sensorsError);

    // vacuumPwm(VACUUM_PWM + abs(u));

    motorsPwmRight(MOTOR_PWM - u);
    motorsPwmLeft(MOTOR_PWM + u);

    HAL_Delay(1);
  }
}

void stop() {
  motorsStop();
  HAL_Delay(500);
  // vacuumPwm(0);

  BLEMessagePush("Stop!");

  action = NONE;
}

void testSensors() {
  while(action == TEST_SENSORS) {
    sensorsTest();
    HAL_Delay(100);
  }
}

/*
void editParams() {
  static uint8_t rx_buffer[32];

  BLEMessagePush("kp = ");
  HAL_UART_Receive(&BLE_BUS, (uint8_t *)rx_buffer, 4, 10000);
  float x = (float)rx_buffer[0] + (float)rx_buffer[1] / 10.0;

  dp = x - kp;

  BLEMessagePush("kd = ");
  HAL_UART_Receive(&BLE_BUS, (uint8_t *)rx_buffer, 4, 10000);
  x = (float)rx_buffer[0] + (float)rx_buffer[1] / 10.0;

  dd = x - kd;
}
*/

void loop(void) {
  switch(action) {
  case RUN: run(); break;

  case STOP: stop(); break;

  case CALIBRATE: sensorsCalibrate(); break;

  case TEST_SENSORS: sensorsTest(); break;

  default: break;
  }

  HAL_Delay(500);
}
