/*
 * echo_task.h
 *
 *  Created on: 2025. 9. 30.
 *      Author: USER
 */

#ifndef ECHO_TASK_H_
#define ECHO_TASK_H_

#include "sockets.h"
#include "FreeRTOS.h"
#include "task.h"

void echo_server_task(void *pvParameters);
void create_echo_task(void);



#endif /* ECHO_TASK_H_ */
