/*
 * ble.cpp
 *
 *  Created on: 29 de ago. de 2026
 *      Author: Usuário
 */

#include "../../Context/GlobalData.hpp"
#include "../../peripherals.h"

#include <cstdio>
#include <cstring>

#include "ble.hpp"

static uint8_t rx_buffer[32];

volatile Action action = NONE;

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
  // Check if the callback was triggered by BLE UART instance, ignore otherwise
  if(huart->Instance == USART1) {
    extern char g;

    // Check the received character, and update the action acordingly
    g = rx_buffer[0];

    // Request UART to receive another character
    HAL_UART_Receive_DMA(&BLE_BUS, (uint8_t *)rx_buffer, 1);
  }
}
}
