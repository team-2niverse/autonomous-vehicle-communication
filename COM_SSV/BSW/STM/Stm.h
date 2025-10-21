/*
 * Stm.h
 *
 *  Created on: 2025. 10. 20.
 *      Author: USER
 */

#ifndef BSW_STM_STM_H_
#define BSW_STM_STM_H_

#include "Ifx_Types.h"

#define CPU_CLOCK_MHZ 100

uint64 Stm_Get_Tick(void);
uint64 Stm_Get_Time_Us(void);
uint64 Stm_Get_Time_Ms(void);

#endif /* BSW_STM_STM_H_ */
