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
#include "Drivers/Sensors/Sensors.hpp"
#include "Drivers/Vacuum/vacuum.hpp"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_uart.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <math.h>

char g = 0;

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

  BLEMessagePush("Starting calibration...");
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

void loop(void) {
  /*
  switch(action) {
  case RUN: BLEMessagePush("Running"); break;

  case STOP: stop(); break;

  default: stop();
  }

  action = NONE;
  */

  char buff[256];

  snprintf(buff, 256, "Action %c\n", g);
  HAL_UART_Transmit(&BLE_BUS, (const uint8_t *)buff, strlen(buff), 10);

  HAL_Delay(500);
}

void run() {
  int dif = sensorsUpdateDirection();

  vacuumPwm(VACUUM_PWM + abs(dif));

  motorsPwmRight(MOTOR_PWM - dif);
  motorsPwmLeft(MOTOR_PWM + dif);
}

void stop() {
  motorsStop();
  HAL_Delay(500);
  vacuumPwm(0);
}
