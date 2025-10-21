/**********************************************************************************************************************
 * file Ultrasonic.c
 * copyright Copyright (C) Infineon Technologies AG 2019
 * Boost Software License - Version 1.0 - August 17th, 2003
 *********************************************************************************************************************/
#include "Ultrasonic.h"
#include "Stm.h"
#include "Isr_Priority.h"
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "Can.h"

void Ultrasonic_Echo_Init(void);
void Ultrasonic_Stm_Init_10us(void);

static volatile unsigned int stm_cnt = 0;
static volatile unsigned int stm_canSendCnt = 0;
static volatile uint64 start_back = 0;
static volatile uint64 start_left = 0;
static volatile uint64 start_right = 0;
static volatile float dist_back = (float)0;
static volatile float dist_left = (float)0;
static volatile float dist_right = (float)0;

float Ultrasonic_Get_Back_Dist(void) {
    return dist_back;
}

float Ultrasonic_Get_Left_Dist(void) {
    return dist_left;
}

float Ultrasonic_Get_Right_Dist(void) {
    return dist_right;
}

uint64 Ultrasonic_Get_Start_Left(void) {
    return start_left;
}

uint64 Ultrasonic_Get_Start_Right(void) {
    return start_right;
}

uint64 Ultrasonic_Get_Start_Back(void) {
    return start_back;
}

void Ultrasonic_Init(void) {
    // Back
    MODULE_P10.IOCR4.B.PC4 = 0x10;  // trig
    MODULE_P02.IOCR0.B.PC0 = 0x02;  // echo: EICR1.EXIS1.2

    // Left
    MODULE_P15.IOCR4.B.PC7 = 0x10;  // trig
    MODULE_P15.IOCR8.B.PC8 = 0x02;  // echo: EICR2.EXIS1.0

    // Right
    MODULE_P15.IOCR0.B.PC0 = 0x10;  // trig
    MODULE_P15.IOCR0.B.PC1 = 0x02;  // echo: EICR3.EXIS1.2

    Ultrasonic_Echo_Init();

    MODULE_P10.OUT.B.P4 = 1;        // Back trig on

    Ultrasonic_Stm_Init_10us();
}

void Ultrasonic_Echo_Init(void) {
    uint16 pw = IfxScuWdt_getSafetyWatchdogPasswordInline();
    IfxScuWdt_clearSafetyEndinitInline(pw);

    // EICR1.EXIS1.2 -> OGU2
    MODULE_SCU.EICR[1].B.EXIS1 = 2;
    MODULE_SCU.EICR[1].B.REN1 = 1;
    MODULE_SCU.EICR[1].B.FEN1 = 1;
    MODULE_SCU.EICR[1].B.EIEN1 = 1;
    MODULE_SCU.EICR[1].B.INP1 = 2;

    // EICR2.EXIS1.0 -> OGU2
    MODULE_SCU.EICR[2].B.EXIS1 = 0;
    MODULE_SCU.EICR[2].B.REN1 = 1;
    MODULE_SCU.EICR[2].B.FEN1 = 1;
    MODULE_SCU.EICR[2].B.EIEN1 = 1;
    MODULE_SCU.EICR[2].B.INP1 = 2;

    // EICR3.EXIS1.2 -> OGU2
    MODULE_SCU.EICR[3].B.EXIS1 = 2;
    MODULE_SCU.EICR[3].B.REN1 = 1;
    MODULE_SCU.EICR[3].B.FEN1 = 1;
    MODULE_SCU.EICR[3].B.EIEN1 = 1;
    MODULE_SCU.EICR[3].B.INP1 = 2;

    MODULE_SCU.IGCR[1].B.IGP0 = 1;  // OGU2 -> SCUERU2

    // set SCUERU2
    MODULE_SRC.SCU.SCUERU[2].B.SRPN = ISR_PRIORITY_ERU2;
    MODULE_SRC.SCU.SCUERU[2].B.TOS = 0;
    MODULE_SRC.SCU.SCUERU[2].B.CLRR = 1;
    MODULE_SRC.SCU.SCUERU[2].B.SRE = 1;

    IfxScuWdt_setSafetyEndinitInline(pw);
}

void Ultrasonic_Stm_Init_10us(void) {
    MODULE_STM1.CMCON.B.MSIZE0 = 31;
    MODULE_STM1.CMCON.B.MSTART0 = 0;
    MODULE_STM1.ICR.B.CMP0OS = 0;

    MODULE_SRC.STM.STM[1].SR[0].B.TOS = 0;
    MODULE_SRC.STM.STM[1].SR[0].B.SRPN = ISR_PRIORITY_STM1_CMP0;
    MODULE_SRC.STM.STM[1].SR[0].B.CLRR = 1;
    MODULE_SRC.STM.STM[1].SR[0].B.SRE = 1;

    MODULE_STM1.ISCR.B.CMP0IRR = 1U;
    MODULE_STM1.ICR.B.CMP0EN = 1U;

    MODULE_STM1.CMP[0].U = (unsigned int)((MODULE_STM1.TIM0.U | ((uint64)MODULE_STM1.CAP.U << 32)) + 10*CPU_CLOCK_MHZ);
}

void Ultrasonic_pollCanSend(void) {
    if (stm_canSendCnt > 10000) {
        // can_send
        uint8 CanData_Ultra[6] = {0, };
        uint8 CanData_left[3] = {0, };
        uint8 CanData_right[3] = {0, };
        uint8 CanData_back[3] = {0, };

        unsigned int left_dist = (unsigned int)dist_left;
        unsigned int right_dist = (unsigned int)dist_right;
        unsigned int back_dist = (unsigned int)dist_back;

        CanData_Ultra[0] = (uint8) (left_dist & 0xFF);
        CanData_Ultra[1] = (uint8) (left_dist >> 8);
        CanData_Ultra[2] = (uint8) (right_dist & 0xFF);
        CanData_Ultra[3] = (uint8) (right_dist >> 8);
        CanData_Ultra[4] = (uint8) (back_dist & 0xFF);
        CanData_Ultra[5] = (uint8) (back_dist >> 8);

        Can_Send_Msg(0x500, CanData_Ultra, 6);

        if (left_dist <= 5)
        {
            CanData_left[0] = 1;
            CanData_left[1] = 1;
            CanData_left[2] = 1;
        }
        else if (left_dist > 5 && left_dist <= 10)
        {
            CanData_left[0] = 1;
            CanData_left[1] = 1;
            CanData_left[2] = 0;
        }
        else if (left_dist > 10 && left_dist <= 20)
        {
            CanData_left[0] = 1;
            CanData_left[1] = 0;
            CanData_left[2] = 0;
        }
        else
        {
            CanData_left[0] = 0;
            CanData_left[1] = 0;
            CanData_left[2] = 0;
        }
        Can_Send_Msg(0x501, CanData_left, 3);

        if (right_dist <= 5)
        {
            CanData_right[0] = 1;
            CanData_right[1] = 1;
            CanData_right[2] = 1;
        }
        else if (right_dist > 5 && right_dist <= 10)
        {
            CanData_right[0] = 1;
            CanData_right[1] = 1;
            CanData_right[2] = 0;
        }
        else if (right_dist > 10 && right_dist <= 20)
        {
            CanData_right[0] = 1;
            CanData_right[1] = 0;
            CanData_right[2] = 0;
        }
        else
        {
            CanData_right[0] = 0;
            CanData_right[1] = 0;
            CanData_right[2] = 0;
        }
        Can_Send_Msg(0x502, CanData_right, 3);

        if (back_dist <= 5)
        {
            CanData_back[0] = 1;
            CanData_back[1] = 1;
            CanData_back[2] = 1;
        }
        else if (back_dist > 5 && back_dist <= 10)
        {
            CanData_back[0] = 1;
            CanData_back[1] = 1;
            CanData_back[2] = 0;
        }
        else if (back_dist > 10 && back_dist <= 20)
        {
            CanData_back[0] = 1;
            CanData_back[1] = 0;
            CanData_back[2] = 0;
        }
        else
        {
            CanData_back[0] = 0;
            CanData_back[1] = 0;
            CanData_back[2] = 0;
        }
        Can_Send_Msg(0x503, CanData_back, 3);

        stm_canSendCnt = 0;
    }
}

IFX_INTERRUPT(Ultrasonic_Stm1_Cmp0_Isr_Handler, 0, ISR_PRIORITY_STM1_CMP0);
IFX_INTERRUPT(Ultrasonic_Eru2_Isr_Handler, 0, ISR_PRIORITY_ERU2);

void Ultrasonic_Stm1_Cmp0_Isr_Handler(void) {
    MODULE_STM1.CMP[0].U = (unsigned int)((MODULE_STM1.TIM0.U | ((uint64)MODULE_STM1.CAP.U << 32)) + 10*CPU_CLOCK_MHZ);
    stm_cnt++;
    stm_canSendCnt++;
    if (stm_cnt == 1) {
        MODULE_P10.OUT.B.P4 = 0;    // Back trig off
        MODULE_P15.OUT.B.P7 = 1;    // Left trig on
    } else if (stm_cnt == 2) {
        MODULE_P15.OUT.B.P7 = 0;    // Left trig off
        MODULE_P15.OUT.B.P0 = 1;    // Right trig on
    } else if (stm_cnt == 3) {
        MODULE_P15.OUT.B.P0 = 0;    // Right trig off
    } else if (stm_cnt == 2000) {
        MODULE_P10.OUT.B.P4 = 1;    // Back trig on
        stm_cnt = 0;
    }
}

void Ultrasonic_Eru2_Isr_Handler(void) {
    if (MODULE_SCU.EIFR.B.INTF3) {
        if (MODULE_P02.IN.B.P0)
            start_back = Stm_Get_Time_Us();
        else
            dist_back = (float)0.0343 * (Stm_Get_Time_Us() - start_back) / 2.0;
        MODULE_SCU.FMR.B.FC3 = 1;
    } else if (MODULE_SCU.EIFR.B.INTF5) {
        if (MODULE_P15.IN.B.P8)
            start_left = Stm_Get_Time_Us();
        else
            dist_left = (float)0.0343 * (Stm_Get_Time_Us() - start_left) / 2.0;
        MODULE_SCU.FMR.B.FC5 = 1;
    } else if (MODULE_SCU.EIFR.B.INTF7) {
        if (MODULE_P15.IN.B.P1)
            start_right = Stm_Get_Time_Us();
        else
            dist_right = (float)0.0343 * (Stm_Get_Time_Us() - start_right) / 2.0;
        MODULE_SCU.FMR.B.FC7 = 1;
    }
}
