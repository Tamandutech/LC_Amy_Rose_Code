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
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_uart.h"

#include "Context/GlobalData.cpp"
#include "Drivers/Battery/Battery.hpp"

#include "Drivers/Vacuum/vacuum.hpp"
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <stdio.h>
#include <string>

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


  HAL_Delay(5000);


  // editParams();

  BLEInitialize();

  BLEMessagePush("RUN (1)\nSTOP (2)\nCALIBRATE (3)\nTEST SENSORS (4)");

  // char batBuffer[16];

  // snprintf(batBuffer, 16, "%f", BatteryVoltageGet());

  // BLEMessagePush(batBuffer);

  // ...s
}


void run() {
  while(action == RUN) {
    sensorsUpdate();

    int u = pidEvaluate(sensorsError);

    vacuumPwm(VACUUM_PWM);

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

void testSensors() {
  while(action == TEST_SENSORS) {
    sensorsTest();
    HAL_Delay(100);
  }
}

/*
void editParams() {
  char PID_Params[128];

  BLEMessagePush("Insira: kp, kd, velocidade, succao");

  HAL_UART_Receive(&BLE_BUS, (uint8_t *)PID_Params, 9, 10000);

  sscanf(PID_Params, "%f %f", &kp, &kd);
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

  HAL_Delay(10);
}
