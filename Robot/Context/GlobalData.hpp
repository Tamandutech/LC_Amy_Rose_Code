/*
 * GlobalData.hpp
 *
 *  Created on: 31 de ago. de 2026
 *      Author: Usuário
 */

#ifndef CONTEXT_GLOBALDATA_HPP_
#define CONTEXT_GLOBALDATA_HPP_

#define MESSAGE_BUFFER_SIZE 64
#define MESSAGE_DELAY       50

#define MOTOR_MAX_PWM 100
#define MOTOR_MIN_PWM 30
#define MOTOR_PWM     50

#define VACUUM_PWM 100


enum Action { NONE, RUN, STOP };

extern volatile Action action;


#endif /* CONTEXT_GLOBALDATA_HPP_ */
