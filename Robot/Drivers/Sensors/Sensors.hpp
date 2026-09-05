/*
 * Sensors.hpp
 *
 *  Created on: 30 de ago. de 2026
 *      Author: Usuário
 */

#ifndef DRIVERS_SENSORS_SENSORS_HPP_
#define DRIVERS_SENSORS_SENSORS_HPP_

void sensorsCalibrate();

void sensorsUpdate();

int sensorsUpdateDirection_TEST();

void sensorsTest();

extern volatile int sensorsError;

#endif /* DRIVERS_SENSORS_SENSORS_HPP_ */
