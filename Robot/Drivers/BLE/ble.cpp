/*
 * ble.cpp
 *
 *  Created on: 29 de ago. de 2026
 *      Author: Usuário
 */

#include "../../Context/GlobalData.hpp"
#include "../../peripherals.h"

#include "../../Context/GlobalData.cpp"
#include "../Battery/Battery.hpp"
#include "../PID/PID.cpp"

#include <cstdio>
#include <cstring>

#include "ble.hpp"

static uint8_t rx_buffer[32] = {0};

void BLEInitialize() {
  HAL_UART_Receive_DMA(&BLE_BUS, (uint8_t *)rx_buffer, 1);
}

// TODO: Fazer uma funcao de transmit melhor para funcionar como logger
void BLEMessagePush(const char *message) {

  static char tx_buffer[MESSAGE_BUFFER_SIZE];

  snprintf(tx_buffer, MESSAGE_BUFFER_SIZE, "%s\n", message);

  HAL_UART_Transmit(&BLE_BUS, (uint8_t *)tx_buffer, strlen(tx_buffer),
                    MESSAGE_DELAY);
}

extern "C" {
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if(huart->Instance == USART1) {
    if(rx_buffer[0] == '1') {
      action = RUN;
    } else if(rx_buffer[0] == '2') {
      action = STOP;
    } else if(rx_buffer[0] == '3') {
      action = CALIBRATE;
    } else if(rx_buffer[0] == '4') {
      action = TEST_SENSORS;
    } else if(rx_buffer[0] == '5') {
      char str[16];
      snprintf(str, 16, "%d", (int)(kd * 100));
      BLEMessagePush("kd: ");
      BLEMessagePush(str);

      char str2[16];
      snprintf(str2, 16, "%d", (int)(kp * 100));
      BLEMessagePush("kp: ");
      BLEMessagePush(str2);
    } else if(rx_buffer[0] == 'p') {
      kp = kp + 0.01;
    } else if(rx_buffer[0] == 'P') {
      kp = kp + 0.1;
    } else if(rx_buffer[0] == 'd') {
      kd = kd + 0.01;
    } else if(rx_buffer[0] == 'D') {
      kd = kd + 0.1;
    } else if(rx_buffer[0] == 'v') {
      kd = kd + 0.01;
    } else if(rx_buffer[0] == 'V') {
      kd = kd + 0.1;
    } else if(rx_buffer[0] == 'b') {
      char batBuffer[16];

      snprintf(batBuffer, 16, "%d", BatteryVoltageGet());

      BLEMessagePush(batBuffer);
    }

    // DO NOT use "else"

    // Request UART to receive another character
    HAL_UART_Receive_DMA(&BLE_BUS, (uint8_t *)rx_buffer, 1);
  }
}
}
