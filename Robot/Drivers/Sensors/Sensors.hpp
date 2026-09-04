/*
 * Sensors.hpp
 *
 *  Created on: 30 de ago. de 2026
 *      Author: Usuário
 */

#ifndef DRIVERS_SENSORS_SENSORS_HPP_
#define DRIVERS_SENSORS_SENSORS_HPP_

void sensorsCalibrate();

int sensorsUpdateDirection();

int sensorsUpdateDirection_TEST();

extern volatile int sensorsError;

#endif /* DRIVERS_SENSORS_SENSORS_HPP_ */
