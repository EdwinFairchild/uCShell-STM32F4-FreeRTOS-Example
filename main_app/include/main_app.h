/*
 * main_app.h
 *
 *  Created on: Feb 3, 2022
 *      Author: amaya
 */

#ifndef MAIN_APP_H_
#define MAIN_APP_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "stdarg.h"
#include "uCShell.h"
void main_app_init(void);
void USART2_callback(void);
#endif /* MAIN_APP_H_ */
