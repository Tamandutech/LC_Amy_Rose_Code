/*
 * Sensors.cpp
 *
 *  Created on: 30 de ago. de 2026
 *      Author: Usuário
 */

#include "../../Context/GlobalData.hpp"
#include "../../peripherals.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "../BLE/ble.hpp"
#include "Sensors.hpp"

#define SAMPLING_DELAY         1
#define SAMPLES                2000
#define SAMPLE_UPDATE_INTERVAL 50

#define MIN_RAW_VALUE 0
#define MAX_RAW_VALUE 4095

#define MAX_SENSOR_VALUE 1000

// TODO: definir valores empiricos
#define MIN_EMPIRICAL_RAW_VALUE 0
#define MAX_EMPIRICAL_RAW_VALUE 4095
// #define DENOMINATOR (MAX_EMPIRICAL_RAW_VALUE - MIN_EMPIRICAL_RAW_VALUE)

#define CENTRAL_POSITION 5500

#define FIRST_CENTRAL 2
#define LAST_CENTRAL  13


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

int sensorValue_[N_IR_SENSORS] = {0};

int position         = 0;
int previousPosition = 0;

volatile int sensorsError = 0;

int sensorsValuesMin[N_IR_SENSORS] = {0};
int sensorsValuesMax[N_IR_SENSORS] = {0};

void sensorsCalibrate() {

  BLEMessagePush("Starting calibration!");
  HAL_Delay(1000);
  BLEMessagePush("3");
  HAL_Delay(1000);
  BLEMessagePush("2");
  HAL_Delay(1000);
  BLEMessagePush("1");


  for(int i = 0; i < N_IR_SENSORS; i++) {
    sensorsValuesMax[i] = MIN_RAW_VALUE;
    sensorsValuesMin[i] = MAX_RAW_VALUE;
  }

  for(int s = 1; s <= SAMPLES; s++) {

    for(int i = 0; i < N_IR_SENSORS; i++) {
      int value           = *irSensorsRawValues[i];
      sensorsValuesMax[i] = std::max(sensorsValuesMax[i], value);
      sensorsValuesMin[i] = std::min(sensorsValuesMin[i], value);
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

  action = NONE;
}

void sensorsReadCalibrated() {

  for(int i = 0; i < N_IR_SENSORS; i++) {
    int value = 0;

    int denominator = sensorsValuesMax[i] - sensorsValuesMin[i];

    value = (((int)*irSensorsRawValues[i] - sensorsValuesMin[i]) *
             MAX_SENSOR_VALUE) /
            denominator;

    value = MAX_SENSOR_VALUE - value;

    // TODO: verificacao do valor entra max e min

    sensorValue_[i] = value;
  }
}

void sensorsUpdate() {
  int weightTotal = 0;
  int total       = 0;

  // bool isOnLine = false;
  sensorsReadCalibrated();

  // TODO: sensor on line
  for(int i = FIRST_CENTRAL; i <= LAST_CENTRAL; i++) {
    int value = sensorValue_[i];

    weightTotal += value * ((i - FIRST_CENTRAL) * MAX_SENSOR_VALUE);
    total += value;
  }

  previousPosition = weightTotal / total;
  position         = previousPosition;

  sensorsError = position - CENTRAL_POSITION;
}

void sensorsTest() {
  for(int i = 0; i < N_IR_SENSORS; i++) {
    char tx_buffer[MESSAGE_BUFFER_SIZE];

    snprintf(tx_buffer, sizeof(tx_buffer), "%d: %d | ", i,
             (int)*irSensorsRawValues[i]);

    HAL_UART_Transmit(&BLE_BUS, (uint8_t *)tx_buffer, strlen(tx_buffer), 1);
  }
}