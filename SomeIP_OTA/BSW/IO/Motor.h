#ifndef BSW_IO_MOTOR_H_
#define BSW_IO_MOTOR_H_

void Motor_Init(void);

void Motor_movChA(int dir);
void Motor_stopChA(void);
void Motor_movChA_PWM(int duty, int dir);

void Motor_movChB(int dir);
void Motor_stopChB(void);
void Motor_movChB_PWM(int duty, int dir);

void Motor_Set_Left(uint8 dir, uint8 duty);
void Motor_Stop_Left(void);

void Motor_Set_Right(uint8 dir, uint8 duty);
void Motor_Stop_Right(void);

#endif /* BSW_IO_MOTOR_H_ */
