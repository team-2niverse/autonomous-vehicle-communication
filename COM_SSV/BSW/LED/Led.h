/*
 * Led.h
 *
 *  Created on: 2025. 10. 20.
 *      Author: USER
 */

#ifndef BSW_LED_LED_H_
#define BSW_LED_LED_H_


void Led_Init(void);
void Led_Set(unsigned char num_LED, unsigned char onOff);
void Led_Toggle(unsigned char num_LED);
void Led_Emergency(unsigned char onOff);

#endif /* BSW_LED_LED_H_ */
