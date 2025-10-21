#include "Diagnostic.h"
#include "Ultrasonic.h"
#include "Motor.h"
#include "Encoder.h"
#include "Stm.h"
#include "Tof.h"

DTC_Info_t DTC_memory[MAX_SUPPORTED_DTCS]; // Recording DTC

static uint8 DTC_Setting_Flag = 1; // Default : ON


static uint16 Encoder_Fault_Left_Cnt = 0;
static uint16 Encoder_Fault_Right_Cnt = 0;

int Check_Ultrasonic_Left_Fault (void);
int Check_Ultrasonic_Right_Fault (void);
int Check_Ultrasonic_Back_Fault (void);
int Check_ToF_Fault (void);
int Check_Encoder_Left_Fault (void);
int Check_Encoder_Right_Fault (void);

void DTC_Manager_Init (void)
{
    DTC_memory[0].dtc_id = DTC_ULTRASONIC_LEFT_FAULT;
    DTC_memory[0].status_byte = 0x00;

    DTC_memory[1].dtc_id = DTC_ULTRASONIC_RIGHT_FAULT;
    DTC_memory[1].status_byte = 0x00;

    DTC_memory[2].dtc_id = DTC_ULTRASONIC_BACK_FAULT;
    DTC_memory[2].status_byte = 0x00;

    DTC_memory[3].dtc_id = DTC_TOF_FAULT;
    DTC_memory[3].status_byte = 0x00;

    DTC_memory[4].dtc_id = DTC_ENCODER_LEFT_FAULT;
    DTC_memory[4].status_byte = 0x00;

    DTC_memory[5].dtc_id = DTC_ENCODER_RIGHT_FAULT;
    DTC_memory[5].status_byte = 0x00;
}

void DTC_Manager_Setting (int OnOff)
{
    if(OnOff == 1) // ON
        DTC_Setting_Flag = 1;
    else // OFF
        DTC_Setting_Flag = 0;
}

void DTC_Manager_Clear (void)
{
    for(int idx=0; idx<MAX_SUPPORTED_DTCS; idx++)
        DTC_memory[idx].status_byte = 0x00;
}

void DTC_Manager_Update (void)
{
    if(DTC_Setting_Flag)
    {
        if (Check_Ultrasonic_Left_Fault())
            DTC_memory[0].status_byte |= (1 << 3);    // confirmedDTC

        if (Check_Ultrasonic_Right_Fault())
            DTC_memory[1].status_byte |= (1 << 3);    // confirmedDTC

        if (Check_Ultrasonic_Back_Fault())
            DTC_memory[2].status_byte |= (1 << 3);    // confirmedDTC

        if (Check_ToF_Fault())
            DTC_memory[3].status_byte |= (1 << 3);    // confirmedDTC

        if (Check_Encoder_Left_Fault())
            DTC_memory[4].status_byte |= (1 << 3);    // confirmedDTC

        if (Check_Encoder_Right_Fault())
            DTC_memory[5].status_byte |= (1 << 3);    // confirmedDTC
    }
}

uint16 DTC_Manager_Count (void)
{
    uint16 dtc_count = 0;
    for (int i = 0; i < MAX_SUPPORTED_DTCS; i++)
        if (DTC_memory[i].status_byte != 0x00)
            dtc_count++;

    return dtc_count;
}

uint32 DTC_Manager_Check (int idx)
{
    if (DTC_memory[idx].status_byte != 0x00)
        return (DTC_memory[idx].dtc_id << 8) | (DTC_memory[idx].status_byte);
    else
        return 0x00000000;
}

int Check_Ultrasonic_Left_Fault (void)
{
    uint64 curr_time = Stm_Get_Time_Us();
    if (curr_time - Ultrasonic_Get_Start_Left() > 1000000) // 1 second
        return 1;
    else
        return 0;
}

int Check_Ultrasonic_Right_Fault (void)
{
    uint64 curr_time = Stm_Get_Time_Us();
    if (curr_time - Ultrasonic_Get_Start_Right() > 1000000) // 1 second
        return 1;
    else
        return 0;
}

int Check_Ultrasonic_Back_Fault (void)
{
    uint64 curr_time = Stm_Get_Time_Us();
    if (curr_time - Ultrasonic_Get_Start_Back() > 1000000) // 1 second
        return 1;
    else
        return 0;
}

int Check_ToF_Fault (void)
{
    uint64 curr_time = Stm_Get_Time_Us();
    if(curr_time - Tof_getTofTime() > 1000000) // 1 second
        return 1;
    else
        return 0;
}

void Encoder_Fault_Left_Init(void)
{
    Encoder_Fault_Left_Cnt= 0;
}

void Encoder_Fault_Right_Init(void)
{
    Encoder_Fault_Right_Cnt = 0;
}

void Encoder_Fault_Left_Inc(void)
{
    Encoder_Fault_Left_Cnt++;
}

void Encoder_Fault_Right_Inc(void)
{
    Encoder_Fault_Right_Cnt++;
}

int Check_Encoder_Left_Fault (void)
{
    if(Motor_getLeftDuty() > 0)
    {
        if(Encoder_Get_Rpm_Left() == 0)
            Encoder_Fault_Left_Cnt++;
    }
    else
    {
        Encoder_Fault_Left_Cnt = 0;
    }

    if(Encoder_Fault_Left_Cnt >= 10)
    {
        Encoder_Fault_Left_Cnt = 0;
        return 1;
    }
    else
        return 0;
}

int Check_Encoder_Right_Fault (void)
{
    if(Motor_getRightDuty() > 0)
    {
        if(Encoder_Get_Rpm_Right() == 0)
            Encoder_Fault_Right_Cnt++;
    }
    else
    {
        Encoder_Fault_Right_Cnt = 0;
    }

    if(Encoder_Fault_Right_Cnt >= 10)
    {
        Encoder_Fault_Right_Cnt = 0;
        return 1;
    }
    return 0;
}

