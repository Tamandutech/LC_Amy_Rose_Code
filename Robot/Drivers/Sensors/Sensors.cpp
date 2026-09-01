/*
 * Sensors.cpp
 *
 *  Created on: 30 de ago. de 2026
 *      Author: Usuário
 */

#include "../../Context/GlobalData.hpp"
#include "../../peripherals.h"

#include <algorithm>
#include <cstdio>
#include <cstring>

#include "../BLE/ble.hpp"
#include "Sensors.hpp"

#define SAMPLING_DELAY         1
#define SAMPLES                2000
#define SAMPLE_UPDATE_INTERVAL 50

#define MIN_RAW_VALUE 0
#define MAX_RAW_VALUE 4095

const volatile uint32_t *const irSensorsRawValues[N_IR_SENSORS] = {
    // Left
    [L_1] = &adc2_buffer[5],
    [L_2] = &adc1_buffer[6],

    // Center
    [C_1]  = &adc1_buffer[7],
    [C_2]  = &adc2_buffer[4],
    [C_3]  = &adc2_buffer[2],
    [C_4]  = &adc2_buffer[1],
    [C_5]  = &adc2_buffer[0],
    [C_6]  = &adc2_buffer[6],
    [C_7]  = &adc2_buffer[7],
    [C_8]  = &adc1_buffer[3],
    [C_9]  = &adc1_buffer[2],
    [C_10] = &adc1_buffer[1],
    [C_11] = &adc1_buffer[0],
    [C_12] = &adc2_buffer[3],

    // Right
    [R_1] = &adc1_buffer[5],
    [R_2] = &adc1_buffer[4]};


uint32_t sensorsValuesMin[N_IR_SENSORS] = {0};
uint32_t sensorsValuesMax[N_IR_SENSORS] = {0};

uint32_t sensorValuesMean[N_IR_SENSORS] = {0};

void sensorsCalibrate() {

  uint32_t samplesMax[N_IR_SENSORS] = {0};
  uint32_t samplesMin[N_IR_SENSORS] = {0};

  for(int i = 0; i < N_IR_SENSORS; i++) {
    sensorsValuesMax[i] = MAX_RAW_VALUE;
    sensorsValuesMin[i] = MIN_RAW_VALUE;
    samplesMax[i]       = MIN_RAW_VALUE;
    samplesMin[i]       = MAX_RAW_VALUE;
  }

  for(int s = 1; s <= SAMPLES; s++) {

    for(int i = 0; i < N_IR_SENSORS; i++) {
      uint32_t value = *irSensorsRawValues[i];
      samplesMax[i]  = std::max(samplesMax[i], value);
      samplesMin[i]  = std::min(samplesMin[i], value);
    }

    if(s % SAMPLE_UPDATE_INTERVAL == 0) {
      for(int i = 0; i < N_IR_SENSORS; i++) {
        sensorsValuesMax[i] = (sensorsValuesMax[i] + samplesMax[i]) / 2;
        sensorsValuesMin[i] = (sensorsValuesMin[i] + samplesMin[i]) / 2;
        samplesMax[i]       = MIN_RAW_VALUE;
        samplesMin[i]       = MAX_RAW_VALUE;
      }
    }

    HAL_Delay(SAMPLING_DELAY);
  }


  for(int i = 0; i < N_IR_SENSORS; i++) {

    char tx_buffer[MESSAGE_BUFFER_SIZE];

    snprintf(tx_buffer, sizeof(tx_buffer), "Min %d: %d | Max %d: %d\n", i,
             (int)sensorsValuesMin[i], i, (int)sensorsValuesMax[i]);

    HAL_UART_Transmit(&BLE_BUS, (uint8_t *)tx_buffer, strlen(tx_buffer),
                      MESSAGE_DELAY);
  }


  for(int i = 0; i < N_IR_SENSORS; i++) {
    sensorValuesMean[i] = (sensorsValuesMax[i] + sensorsValuesMin[i]) / 2;
  }
}


int sensorsUpdateDirection() {
  int left_weight = 0, right_weight = 0;

  // read left side sensors
  for(int i = N_IR_SENSORS / 2 - 1; i >= 0; i--) {
    int value = *irSensorsRawValues[i];

    if(value < (int)sensorValuesMean[i]) left_weight += N_IR_SENSORS / 2 - i;
  }

  // read right side sensors
  for(int i = N_IR_SENSORS / 2; i < N_IR_SENSORS; i++) {
    int value = *irSensorsRawValues[i];

    if(value < (int)sensorValuesMean[i])
      left_weight += i - N_IR_SENSORS / 2 + 1;
  }

  int dif = right_weight - left_weight;

  return dif;
}