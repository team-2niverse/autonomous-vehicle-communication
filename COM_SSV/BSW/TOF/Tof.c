#include "Tof.h"
#include "Ifx_Types.h"
#include "Can.h"
#include "Stm.h"

static volatile unsigned int g_tofDist = 100;
static volatile uint64 g_lastMeasuredTime = 0;
static volatile unsigned char g_canSend = 0;

unsigned int Tof_getDist(void) {
    return g_tofDist;
}

void Tof_setDist(uint8 *rxData) {
    unsigned int tofVal = rxData[2] << 16 | rxData[1] << 8 | rxData[0];
    // unsigned char dis_status = rxData[3];
    uint16 signal_strength =  rxData[5] << 8 | rxData[4];

    if (signal_strength != 0) {
        g_tofDist = tofVal;
        g_lastMeasuredTime = Stm_Get_Time_Us();
        g_canSend = 1;
    }
}

uint64 Tof_getTofTime(void) {
    return g_lastMeasuredTime;
}

void Tof_pollCanSend(void) {
    if (g_canSend) {
        unsigned int front_dist = g_tofDist / 10;
        unsigned char CanData_Tof[2] = {0, };
        unsigned char CanData_front[3] = {0, };

        CanData_Tof[0] = (uint8) (front_dist & 0xFF);
        CanData_Tof[1] = (uint8) (front_dist >> 8);

        if (front_dist <= 5)
        {
            CanData_front[0] = 1;
            CanData_front[1] = 1;
            CanData_front[2] = 1;
        }
        else if (front_dist > 5 && front_dist <= 10)
        {
            CanData_front[0] = 1;
            CanData_front[1] = 1;
            CanData_front[2] = 0;
        }
        else if (front_dist > 10 && front_dist <= 20)
        {
            CanData_front[0] = 1;
            CanData_front[1] = 0;
            CanData_front[2] = 0;
        }
        else
        {
            CanData_front[0] = 0;
            CanData_front[1] = 0;
            CanData_front[2] = 0;
        }
        Can_Send_Msg(0x700, CanData_Tof, 2);
        Can_Send_Msg(0x504, CanData_front, 3);

        g_canSend = 0;
    }
}
