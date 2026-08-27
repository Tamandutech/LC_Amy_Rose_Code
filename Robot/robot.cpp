/*
 * robot.cpp
 *
 *  Created on: Aug 27, 2026
 *      Author: Kelvin Novais
 */

#include "peripherals.h"
#include "robot.h"

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

  // ...
}

void loop(void) {
  // ...
}


