#include "Led.h"
#include "Can.h"
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

static unsigned char g_emergency = 0;

void Led_Init(void){
    IfxPort_setPinMode(&MODULE_P00, 5, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(&MODULE_P00, 6, IfxPort_Mode_outputPushPullGeneral);
    MODULE_P00.OUT.B.P5 = 1;
    MODULE_P00.OUT.B.P6 = 1;

    MODULE_P23.IOCR0.B.PC1 = 0x10;
    MODULE_P23.IOCR0.B.PC3 = 0x10;
    MODULE_P23.OUT.B.P1 = 0;
    MODULE_P23.OUT.B.P3 = 0;
}

void Led_Set(unsigned char num_LED, unsigned char onOff){
    if (g_emergency)
        return;
    if (num_LED == 1){
        MODULE_P23.OUT.B.P1 = onOff;
    }
    else if (num_LED == 2){
        MODULE_P23.OUT.B.P3 = onOff;
    }

    uint8 CanData_Led[2] = {0, };
    CanData_Led[0] = MODULE_P23.OUT.B.P1;
    CanData_Led[1] = MODULE_P23.OUT.B.P3;
    Can_Send_Msg(0x400, CanData_Led, 2);
}

void Led_Toggle(unsigned char num_LED){
    if (g_emergency)
        return;
    if (num_LED == 1){
        MODULE_P23.OUT.B.P1 ^= 1;
    }
    else if (num_LED == 2){
        MODULE_P23.OUT.B.P3 ^= 1;
    }

    uint8 CanData_Led[2] = {0, };
    CanData_Led[0] = MODULE_P23.OUT.B.P1;
    CanData_Led[1] = MODULE_P23.OUT.B.P3;
    Can_Send_Msg(0x400, CanData_Led, 2);
}

void Led_Emergency(unsigned char onOff) {
    g_emergency = onOff;
    MODULE_P23.OUT.B.P1 = onOff;
    MODULE_P23.OUT.B.P3 = onOff;

    uint8 CanData_Led[2] = {0, };
    CanData_Led[0] = onOff;
    CanData_Led[1] = onOff;
    Can_Send_Msg(0x400, CanData_Led, 2);
}
