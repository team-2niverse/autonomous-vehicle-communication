#include "isr_priority.h"
#include "my_stdio.h"
#include "GPIO.h"
#include "can.h"
#include "Motor.h"

static unsigned int g_TofValue = 0;

static unsigned char aeb = 0;

IFX_INTERRUPT(TofIsrHandler, 0, ISR_PRIORITY_CAN_RX);
void TofIsrHandler(void)
{
    unsigned int rxID;
    unsigned char rxData[8] = {0,};
    int rxLen;
    Can_RecvMsg(&rxID, rxData, &rxLen);
    unsigned char dis_status = rxData[3];
    unsigned short signal_strength = rxData[5] << 8 | rxData[4];

    if (signal_strength != 0) {
        g_TofValue = rxData[2] << 16 | rxData[1] << 8 | rxData[0];
        if (aeb == 0 && g_TofValue < 500 && MODULE_P10.OUT.B.P1 == 0 && MODULE_P10.OUT.B.P2 == 0) {
            Motor_Stop_Left();
            Motor_Stop_Right();
            aeb = 1;
        } else if (aeb == 1 && g_TofValue > 400) {
            aeb = 0;
        }
    }
}

unsigned int Tof_GetValue(void)
{
    return g_TofValue;
}

unsigned char Tof_GetAeb(void) {
    return aeb;
}
