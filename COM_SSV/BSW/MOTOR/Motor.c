#include "Motor.h"
#include "Tof.h"
#include "Encoder.h"
#include "Led.h"
#include "Gtm_Atom_Pwm.h"
#include "IfxCpu.h"

/*
chA → Left Motor Control
chB → Right Motor Control
 */

static volatile uint8 g_leftDir = 1;
static volatile uint8 g_leftDuty = 0;
static volatile uint8 g_rightDir = 1;
static volatile uint8 g_rightDuty = 0;

static volatile unsigned char g_isAeb = 0;
static volatile int g_aebOffset = 100;

void Motor_init(void) {
    MODULE_P10.IOCR0.B.PC1 = 0x10;  // chA: initialize direction
    MODULE_P02.IOCR4.B.PC7 = 0x10;  // chA: initialize brake

    MODULE_P10.IOCR0.B.PC2 = 0x10;  // chB: initialize direction
    MODULE_P02.IOCR4.B.PC6 = 0x10;  // chB: initialize brake

    MODULE_P02.OUT.B.P7 = 1;        // chA: set brake
    MODULE_P02.OUT.B.P6 = 1;        // chB: set brake

    MODULE_P10.OUT.B.P1 = 0;        // chA: set direction
    MODULE_P10.OUT.B.P2 = 0;        // chB: set direction

    Gtm_Atom_Pwm_Init();

    Gtm_Atom_Pwm_Set_Duty_Cycle_A(0); //duty rate 0~255
    Gtm_Atom_Pwm_Set_Duty_Cycle_B(0);
}

void Motor_stopLeft(void) {
    MODULE_P02.OUT.B.P7 = 1;
    MODULE_P10.OUT.B.P1 = 0;
    Gtm_Atom_Pwm_Set_Duty_Cycle_A(0);
}

void Motor_stopRight(void) {
    MODULE_P02.OUT.B.P6 = 1;
    MODULE_P10.OUT.B.P2 = 0;
    Gtm_Atom_Pwm_Set_Duty_Cycle_B(0);
}

void Motor_driveLeft(uint8 dir, uint8 duty) {

    if (MODULE_P02.OUT.B.P7 == 0) {
        if (MODULE_P10.OUT.B.P1 != dir) {
            Motor_stopLeft();
            MODULE_P10.OUT.B.P1 = dir;
            MODULE_P02.OUT.B.P7 = 0;
        }
    } else {
        MODULE_P10.OUT.B.P1 = dir;
        MODULE_P02.OUT.B.P7 = 0;
    }
    Gtm_Atom_Pwm_Set_Duty_Cycle_A((uint32)(duty*3.92157));
}

void Motor_driveRight(uint8 dir, uint8 duty) {

    if (MODULE_P02.OUT.B.P6 == 0) {
        if (MODULE_P10.OUT.B.P2 != dir) {
            Motor_stopRight();
            MODULE_P10.OUT.B.P2 = dir;
            MODULE_P02.OUT.B.P6 = 0;
        }
    } else {
        MODULE_P10.OUT.B.P2 = dir;
        MODULE_P02.OUT.B.P6 = 0;
    }
    Gtm_Atom_Pwm_Set_Duty_Cycle_B((uint32)(duty*3.92157));
}



void Motor_pollDrive(void) {
    unsigned int dist_front = Tof_getDist();
    unsigned int rpm_avg = (Encoder_Get_Rpm_Left() + Encoder_Get_Rpm_Right()) >> 1;
    if (g_isAeb == 0 && dist_front < g_aebOffset + rpm_avg &&
            MODULE_P10.OUT.B.P1 == 0 && MODULE_P10.OUT.B.P2 == 0) {
        Motor_stopLeft();
        Motor_stopRight();
        g_isAeb = 1;
        Led_Emergency(1);
    } else if (g_isAeb == 1 && dist_front > 400) {
        g_isAeb = 0;
        Led_Emergency(0);
    }

    if (!(g_isAeb && g_leftDir == 0 && g_leftDuty > 0))
        Motor_driveLeft(g_leftDir, g_leftDuty);
    if (!(g_isAeb && g_rightDir == 0 && g_rightDuty > 0))
        Motor_driveRight(g_rightDir, g_rightDuty);
}

uint8 Motor_getLeftDir(void) {
    return g_leftDir;
}

uint8 Motor_getLeftDuty(void) {
    return g_leftDuty;
}

void Motor_setLeft(uint8 dir, uint8 duty) {
    g_leftDir = dir;
    g_leftDuty = duty;
}

uint8 Motor_getRightDir(void) {
    return g_rightDir;
}

uint8 Motor_getRightDuty(void) {
    return g_rightDuty;
}

void Motor_setRight(uint8 dir, uint8 duty) {
    g_rightDir = dir;
    g_rightDuty = duty;
}

unsigned char Motor_getAeb(void) {
    return g_isAeb;
}

void Motor_setAeb(unsigned char logical) {
    g_isAeb = logical;
}

int Motor_getAebOffset(void) {
    return g_aebOffset;
}

void Motor_setAebOffset(int offset) {
    g_aebOffset = offset;
}
