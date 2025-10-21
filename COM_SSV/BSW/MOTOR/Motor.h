#ifndef BSW_MOTOR_MOTOR_H_
#define BSW_MOTOR_MOTOR_H_

#include "Ifx_Types.h"

// Function Prototypes
void Motor_init(void);

void Motor_pollDrive(void);

uint8 Motor_getLeftDir(void);
uint8 Motor_getRightDir(void);

uint8 Motor_getLeftDuty(void);
uint8 Motor_getRightDuty(void);

void Motor_setLeft(uint8 dir, uint8 duty);
void Motor_setRight(uint8 dir, uint8 duty);

unsigned char Motor_getAeb(void);
void Motor_setAeb(unsigned char logical);

int Motor_getAebOffset(void);
void Motor_setAebOffset(int offset);

#endif /* BSW_MOTOR_MOTOR_H_ */
