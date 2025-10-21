#ifndef BSW_ULTRASONIC_ULTRASONIC_H_
#define BSW_ULTRASONIC_ULTRASONIC_H_

#include "Ifx_Types.h"

// Function Prototype
void Ultrasonic_Init(void);
float Ultrasonic_Get_Back_Dist(void);
float Ultrasonic_Get_Left_Dist(void);
float Ultrasonic_Get_Right_Dist(void);
uint64 Ultrasonic_Get_Start_Left(void);
uint64 Ultrasonic_Get_Start_Right(void);
uint64 Ultrasonic_Get_Start_Back(void);
void Ultrasonic_pollCanSend(void);

#endif /* BSW_ULTRASONIC_ULTRASONIC_H_ */
