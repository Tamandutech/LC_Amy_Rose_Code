/*
 * Motors.cpp
 *
 *  Created on: 30 de ago. de 2026
 *      Author: Usuário
 */

#include "../../Context/GlobalData.hpp"
#include "../../peripherals.h"

#include "Motors.hpp"

#include <algorithm>

void motorsInitialize() {
  HAL_TIM_PWM_Start(&MOTOR_LEFT_TIMER, MOTOR_LEFT_CHANNEL);
  HAL_TIM_PWM_Start(&MOTOR_RIGHT_TIMER, MOTOR_RIGHT_CHANNEL);
}

void motorsPwmLeft(int duty) {
  if(duty > 0) {
    HAL_GPIO_WritePin(MOTOR_LEFT_DIRECTION_PORT, MOTOR_LEFT_DIRECTION_PIN,
                      GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(MOTOR_LEFT_DIRECTION_PORT, MOTOR_LEFT_DIRECTION_PIN,
                      GPIO_PIN_SET);
    duty = -duty;
  }

  duty = std::min(duty, MOTOR_MAX_PWM);
  duty = std::max(duty, MOTOR_MIN_PWM);

  __HAL_TIM_SET_COMPARE(&MOTOR_LEFT_TIMER, MOTOR_LEFT_CHANNEL, duty);
}

void motorsPwmRight(int duty) {

  if(duty > 0) {
    HAL_GPIO_WritePin(MOTOR_RIGHT_DIRECTION_PORT, MOTOR_RIGHT_DIRECTION_PIN,
                      GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(MOTOR_RIGHT_DIRECTION_PORT, MOTOR_RIGHT_DIRECTION_PIN,
                      GPIO_PIN_RESET);
    duty = -duty;
  }

  duty = std::min(duty, MOTOR_MAX_PWM);
  duty = std::max(duty, MOTOR_MIN_PWM);

  __HAL_TIM_SET_COMPARE(&MOTOR_RIGHT_TIMER, MOTOR_RIGHT_CHANNEL, duty);
}

void motorsStop() {
  // TODO: adicionar mecanica de freio

  __HAL_TIM_SET_COMPARE(&MOTOR_RIGHT_TIMER, MOTOR_RIGHT_CHANNEL, 0);
  __HAL_TIM_SET_COMPARE(&MOTOR_LEFT_TIMER, MOTOR_LEFT_CHANNEL, 0);
}
