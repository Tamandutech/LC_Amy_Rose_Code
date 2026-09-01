/*
 * Motors.hpp
 *
 *  Created on: 30 de ago. de 2026
 *      Author: Usuário
 */

#ifndef DRIVERS_MOTORS_MOTORS_HPP_
#define DRIVERS_MOTORS_MOTORS_HPP_

void motorsInitialize();

void motorsPwmLeft(int duty);

void motorsPwmRight(int duty);

void motorsStop();

#endif /* DRIVERS_MOTORS_MOTORS_HPP_ */
