#include <gpt12.h>
#include <Ifx_reg.h>
#include <Ifx_Types.h>
#include <IfxGpt12.h>
#include <IfxPort.h>
#include "Motor.h"
#include "Bsp.h"
#include "isr_priority.h"
#include "gtm_atom_pwm.h"
#include "Tof.h"

void Motor_Init(void) {
    MODULE_P10.IOCR0.B.PC1 = 0x10;  // chA: initialize direction
    MODULE_P02.IOCR4.B.PC7 = 0x10;  // chA: initialize brake

    MODULE_P10.IOCR0.B.PC2 = 0x10;  // chB: initialize direction
    MODULE_P02.IOCR4.B.PC6 = 0x10;  // chB: initialize brake

    MODULE_P02.OUT.B.P7 = 1;        // chA: set brake
    MODULE_P02.OUT.B.P6 = 1;        // chB: set brake

    MODULE_P10.OUT.B.P1 = 1;        // chA: set direction
    MODULE_P10.OUT.B.P2 = 1;        // chB: set direction

    GtmAtomPwm_Init();

    GtmAtomPwmA_SetDutyCycle(0); //duty rate 0~255
    GtmAtomPwmB_SetDutyCycle(0);
}

///* 1: 정방향, 2: 역방향 */
void Motor_movChA(int dir)
{
    if(dir)
    {
        MODULE_P10.OUT.B.P1 = 1; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }
    else {
        MODULE_P10.OUT.B.P1 = 0; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }
    MODULE_P02.OUT.B.P7 = 0;   /* 모터 Brake 해제 (1: 정지, 0: PWM-A에 따라 동작) */
    GtmAtomPwm_SetDutyCycle(1000); /* 100% PWM duty  */
}

void Motor_stopChA(void)
{
    MODULE_P02.OUT.B.P7 = 1;   /* 모터 Brake 신호 인가 (1: 정지, 0: PWM-A에 따라 동작) */
}


///* 1: 정방향, 0: 역방향 */
void Motor_movChA_PWM(int duty, int dir)
{
//    GtmAtomPwm_SetDutyCycle(duty);
    GtmAtomPwmA_SetDutyCycle(duty*10);
    if(dir)
    {
        MODULE_P10.OUT.B.P1 = 1; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }
    else {
        MODULE_P10.OUT.B.P1 = 0; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }

    MODULE_P02.OUT.B.P7 = 0;   /* 모터 Brake 해제 (1: 정지, 0: PWM-A에 따라 동작) */
}

///* 1: 정방향, 2: 역방향 */
void Motor_movChB(int dir)
{
    if(dir)
    {
        MODULE_P10.OUT.B.P2 = 1; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }
    else {
        MODULE_P10.OUT.B.P2 = 0; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }
    MODULE_P02.OUT.B.P6 = 0;   /* 모터 Brake 해제 (1: 정지, 0: PWM-A에 따라 동작) */
    GtmAtomPwm_SetDutyCycle(1000); /* 100% PWM duty  */
}

void Motor_stopChB(void)
{
    MODULE_P02.OUT.B.P6 = 1;   /* 모터 Brake 신호 인가 (1: 정지, 0: PWM-A에 따라 동작) */
}


///* 1: 정방향, 0: 역방향 */
void Motor_movChB_PWM(int duty, int dir)
{
//    GtmAtomPwm_SetDutyCycle(duty);
    GtmAtomPwmB_SetDutyCycle(duty*10);

    if(dir)
    {
        MODULE_P10.OUT.B.P2 = 1; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }
    else {
        MODULE_P10.OUT.B.P2 = 0; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }

    MODULE_P02.OUT.B.P6 = 0;   /* 모터 Brake 해제 (1: 정지, 0: PWM-A에 따라 동작) */
}




/*
chA → Left Motor Control
chB → Right Motor Control
 */

void Motor_Set_Left(uint8 dir, uint8 duty) {
    if (Tof_GetAeb() && dir == 0 && duty > 0) {
            return;
    }

    if (MODULE_P02.OUT.B.P7 == 0) {
        if (MODULE_P10.OUT.B.P1 != dir) {
            Motor_Stop_Left();
            MODULE_P10.OUT.B.P1 = dir;
            MODULE_P02.OUT.B.P7 = 0;
        }
    } else {
        MODULE_P10.OUT.B.P1 = dir;
        MODULE_P02.OUT.B.P7 = 0;
    }
    GtmAtomPwmA_SetDutyCycle((uint32)(duty*3.92157));
}

void Motor_Stop_Left(void) {
    GtmAtomPwmA_SetDutyCycle(0);
    MODULE_P02.OUT.B.P7 = 1;
    MODULE_P10.OUT.B.P1 = 1;
}

void Motor_Set_Right(uint8 dir, uint8 duty) {
    if (Tof_GetAeb() && dir == 0 && duty > 0) {
            return;
    }

    if (MODULE_P02.OUT.B.P6 == 0) {
        if (MODULE_P10.OUT.B.P2 != dir) {
            Motor_Stop_Right();
            MODULE_P10.OUT.B.P2 = dir;
            MODULE_P02.OUT.B.P6 = 0;
        }
    } else {
        MODULE_P10.OUT.B.P2 = dir;
        MODULE_P02.OUT.B.P6 = 0;
    }
    GtmAtomPwmB_SetDutyCycle((uint32)(duty*3.92157));
}

void Motor_Stop_Right(void) {
    GtmAtomPwmB_SetDutyCycle(0);
    MODULE_P02.OUT.B.P6 = 1;
    MODULE_P10.OUT.B.P2 = 1;
}
