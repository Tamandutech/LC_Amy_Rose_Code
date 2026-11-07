/*
 * vacuum.cpp
 *
 *  Created on: Oct 15, 2026
 *      Author: felipe
 */

#include "../Vacuum/vacuum.hpp"

#include "../../peripherals.h"


void vacuumInitialize() { HAL_TIM_PWM_Start(&VACUUM_TIMER, VACUUM_CHANNEL); }

void vacuumPwm(int duty) {

  __HAL_TIM_SET_COMPARE(&VACUUM_TIMER, VACUUM_CHANNEL, duty);
}
