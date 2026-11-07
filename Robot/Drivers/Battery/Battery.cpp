/*
 * Battery.cpp
 *
 *  Created on: Nov 5, 2026
 *      Author: felipe
 */

#include "../../peripherals.h"
#include <cstdint>


volatile uint32_t *rawVoltage    = &adc2_buffer[8];
volatile uint32_t *rawRefVoltage = &adc1_buffer[8];

float BatteryVoltageGet() {

  const uint8_t samples = 16;
  float         result  = 0;

  for(uint8_t i = 0; i < samples; i++) {

    float vref = 1.21F * 4095.0F / *rawRefVoltage;

    float vbat = (*rawVoltage * vref / 4095.0F) * 5.6875F;

    result += vbat;
  }

  return (result / samples);
}
