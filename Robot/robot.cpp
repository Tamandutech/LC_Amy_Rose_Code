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
  BLEInitialize();

  BLEMessagePush("Starting calibration!");
  HAL_Delay(1000);
  BLEMessagePush("3");
  HAL_Delay(1000);
  BLEMessagePush("2");
  HAL_Delay(1000);
  BLEMessagePush("1");
  HAL_Delay(1000);

  sensorsCalibrate();
  BLEMessagePush("Calibrated!");
  HAL_Delay(500);

  BLEMessagePush("RUN (1)\nSTOP (2)");

  // ...
}

void run() {
  while(action == RUN) {
    int u = pidEvaluate(sensorsError);

    vacuumPwm(VACUUM_PWM + abs(u));

    motorsPwmRight(MOTOR_PWM - u);
    motorsPwmLeft(MOTOR_PWM + u);

    HAL_Delay(1);
  }
}

void stop() {
  motorsStop();
  HAL_Delay(500);
  vacuumPwm(0);

  BLEMessagePush("Stop!");

  action = NONE;
}

void loop(void) {
  switch(action) {
  case RUN: run(); break;

  case STOP: stop(); break;

  default: break;
  }

  HAL_Delay(500);
}
