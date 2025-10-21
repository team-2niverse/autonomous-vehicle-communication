#include <string.h>
#include <stdio.h>
#include "Platform_Types.h"
#include "lwip/pbuf.h"
//#include "my_stdio.h"

#include "Uds_config.h"
#include "Uds.h"

#include "Can.h" // LED, Aeb offset, ToF 값

#include "Tof.h"
#include "Motor.h"

#include "Ultrasonic.h" // Ultrasonic 값
#include "Encoder.h" //rpm 값
#include "UART_Logging.h"
#include "Stm.h"
#include "Diagnostic.h"
#include "Isr_Priority.h"


char strbuf[64];

static uint8 UDS_SID_list[MAX_SUPPORTED_SIDS] = {0x10, 0x3E, 0x22, 0x2E, 0x19, 0x14, 0x85};

//Initialize session to default session
static UDS_Session_t Current_Session = DEFAULT_SESSION;
static unsigned int Session_Time_Out_Cnt = 0;


//UDS SERVICE
/* Diagnostic Session Control(0x10) */
static uint16 DiagnosticSessionControl (uint8 *request_uds, uint8 *response_buffer);
/* Tester Present(0x3E) */
static uint16 TesterPresent (uint8 *request_uds, uint8 *response_buffer);
/* Read Data by Identifier(0x22) */
static uint16 ReadDataByIdentifier (uint8 *request_uds, uint8 *response_buffer);
/* Write Data by Identifier(0x2E) */
static uint16 WriteDataByIentifier (uint8 *request_uds, uint8 *response_buffer);
/* Read DTC Information(0x19) */
static uint16 ReadDTCInformation (uint8 *request_uds, uint8 *response_buffer);
/* Clear Diagnostic Information(0x14) */
static uint16 ClearDiagnosticInformation (uint8 *request_uds, uint8 *response_buffer);
/* Control DTC Setting(0x85) */
static uint16 ControlDTCSetting (uint8 *request_uds, uint8 *response_buffer);

void Uds_Init(void) {
    DTC_Manager_Init();
    UDS_Stm1_Init_100ms();
}

// Self-Diagnostic(100ms) & Session-Check(1000ms)
void UDS_Stm1_Init_100ms (void) {
    MODULE_STM2.CMCON.B.MSIZE0 = 31;
    MODULE_STM2.CMCON.B.MSTART0 = 0;
    MODULE_STM2.ICR.B.CMP0OS = 0;

    MODULE_SRC.STM.STM[2].SR[0].B.TOS = 0;
    MODULE_SRC.STM.STM[2].SR[0].B.SRPN = ISR_PRIORITY_STM2_CMP0;
    MODULE_SRC.STM.STM[2].SR[0].B.CLRR = 1;
    MODULE_SRC.STM.STM[2].SR[0].B.SRE = 1;

    MODULE_STM2.ISCR.B.CMP0IRR = 1U;
    MODULE_STM2.ICR.B.CMP0EN = 1U;

    MODULE_STM2.CMP[0].U = (unsigned int)((MODULE_STM2.TIM0.U | ((uint64)MODULE_STM2.CAP.U << 32)) + 100000*CPU_CLOCK_MHZ);
}


//void UDS_Session_Timer_Init(int setting_time);

uint16 UDS_HandleRequest (struct pbuf *request_pbuf, uint8 *response_buffer)
{

    // Tester Present 외에도 다른 진단 요청을 받으면 타이머 리셋 필요
    if(Current_Session == EXTENDED_SESSION)
    {
        Session_Time_Out_Cnt = 0;
    }

    uint8 *doip_payload = (uint8*) request_pbuf->payload; //doIP 메세지 전체
    uint8 *request_uds = &doip_payload[12]; // UDS msg 부분만
    uint8 service_id = request_uds[0];

    int sid_check_flag = 0;
    for(int i=0; i<MAX_SUPPORTED_SIDS; i++)
    {
        if(service_id == UDS_SID_list[i])
            sid_check_flag = 1;
    }

    if(sid_check_flag == 0)
    {
        printUART("Service Failed, NRC: 0x11\r\n");
        response_buffer[0] = 0x7F;          // Negative Response
        response_buffer[1] = service_id;    // SID
        response_buffer[2] = 0x11;          // NRC : serviceNotSupported
        return 3;
    }

    // Check Supported Session (Extended Session)
    if(service_id == 0x2E || service_id == 0x14 || service_id == 0x85)
    {
        if(Current_Session == DEFAULT_SESSION)
        {
            printUART("Sevice Failed, NRC: 0x7F\r\n");
            response_buffer[0] = 0x7F;          // Negative Response
            response_buffer[1] = service_id;    // SID
            response_buffer[2] = 0x7F;          // NRC: serviceNotSupportedInActiveSession
            return 3;
        }
    }

    switch (service_id)
    {
        case 0x10 :
            printUART("Service 0x10 Activated\r\n");
            return DiagnosticSessionControl(request_uds, response_buffer);
        case 0x3E :
            printUART("Service 0x3E Activated\r\n");
            return TesterPresent(request_uds, response_buffer);
        case 0x22 :
            printUART("Service 0x22 Activated\r\n");
            return ReadDataByIdentifier(request_uds, response_buffer);
        case 0x2E :
            printUART("Service 0x2E Activation\r\n");
            return WriteDataByIentifier(request_uds, response_buffer);
        case 0x19 :
            printUART("Service 0x19 Activation\r\n");
            return ReadDTCInformation(request_uds, response_buffer);
        case 0x14 :
            printUART("Service 0x14 Activation\r\n");
            return ClearDiagnosticInformation(request_uds, response_buffer);
        case 0x85 :
            printUART("Service 0x85 Activation\r\n");
            return ControlDTCSetting(request_uds, response_buffer);
    }

    return 0;
}

/* Diagnostic Session Control(0x10) */
static uint16 DiagnosticSessionControl (uint8 *request_uds, uint8 *response_buffer)
{
    uint8 subfunction = request_uds[1];
    response_buffer[0] = 0x50;

    if(subfunction == DEFAULT_SESSION)
    {
        printUART("Session Changed to DEFAULT(0x01)\r\n");
        Current_Session = DEFAULT_SESSION;
        Session_Time_Out_Cnt = 0;
        response_buffer[1] = 0x01;  // sub-function
        response_buffer[2] = 0x00;  // P2Server_max = 50ms
        response_buffer[3] = 0x32;
        response_buffer[4] = 0x13;  // P2*Server_max = 5000ms
        response_buffer[5] = 0x88;
        return 6;
    }
    else if(subfunction == EXTENDED_SESSION)
    {
        printUART("Session Changed to EXTENDED(0x03)\r\n");
        Current_Session = EXTENDED_SESSION;
        Session_Time_Out_Cnt = 0;
        response_buffer[1] = 0x03;  // sub-function
        response_buffer[2] = 0x00;  // P2Server_max = 50ms
        response_buffer[3] = 0x32;
        response_buffer[4] = 0x13;  // P2*Server_max = 5000ms
        response_buffer[5] = 0x88;
        return 6;
    }
    else
    {
        printUART("Change Session Failed, NRC: 0x12\r\n");
        response_buffer[0] = 0x7F;  // Negative Response
        response_buffer[1] = 0x10;  // SID
        response_buffer[2] = 0x12;  // NRC: requestOutOfRange(sub-function)
        return 3;
    }
}

/* Tester Present(0x3E) */
static uint16 TesterPresent (uint8 *request_uds, uint8 *response_buffer)
{
//    uint8 subfunction = request_uds[1]; //0x80
//    if(subfunction != 0x80)
//    {
//        my_printf("Tester Present Failed, NRC: 0x12\n");
//        response_buffer[0] = 0x7F;  // Negative Response
//        response_buffer[1] = 0x10;  // SID
//        response_buffer[2] = 0x12;  // NRC: requestOutOfRange(sub-function)
//        return 3;
//    }

    return 0;
}

/* Read Data by Identifier(0x22) */
static uint16 ReadDataByIdentifier (uint8 *request_uds, uint8 *response_buffer)
{
    uint16 data_id = (request_uds[1] << 8) | request_uds[2];

    response_buffer[0] = 0x62;
    response_buffer[1] = request_uds[1];
    response_buffer[2] = request_uds[2];

    switch (data_id)
    {
        case 0xF190 :
            printUART("Read Data 0xF190 Activated\r\n");
            memcpy(&response_buffer[3], DID_F190_VIN_NUMBER, strlen(DID_F190_VIN_NUMBER));
            return 3 + strlen(DID_F190_VIN_NUMBER);
        case 0xF18C :
            printUART("Read Data 0xF18C Activated\r\n");
            memcpy(&response_buffer[3], DID_F18C_ECU_SERIAL_NUMBER, strlen(DID_F18C_ECU_SERIAL_NUMBER));
            return 3 + strlen(DID_F18C_ECU_SERIAL_NUMBER);
        case 0xF18A :
            printUART("Read Data 0xF18A Activated\r\n");
            memcpy(&response_buffer[3], DID_F18A_SUPPLIER_ID, strlen(DID_F18A_SUPPLIER_ID));
            return 3 + strlen(DID_F18A_SUPPLIER_ID);
        case 0xF193 :
            printUART("Read Data 0xF193 Activated\r\n");
            memcpy(&response_buffer[3], DID_F193_HW_VERSION, strlen(DID_F193_HW_VERSION));
            return 3 + strlen(DID_F193_HW_VERSION);
        case 0xF195 :
            printUART("Read Data 0xF195 Activated\r\n");
            memcpy(&response_buffer[3], DID_F195_SW_VERSION, strlen(DID_F195_SW_VERSION));
            return 3 + strlen(DID_F195_SW_VERSION);
        case 0x0001 :
            printUART("Read Data 0x0001(Ultrasonic) Activated\r\n");
            uint16 ultra_back = (uint16) Ultrasonic_Get_Back_Dist();
            uint16 ultra_left = (uint16) Ultrasonic_Get_Left_Dist();
            uint16 ultra_right = (uint16) Ultrasonic_Get_Right_Dist();
            response_buffer[3] = (uint8) (ultra_left >> 8);
            response_buffer[4] = (uint8) (ultra_left & 0xFF);
            response_buffer[5] = (uint8) (ultra_right >> 8);
            response_buffer[6] = (uint8) (ultra_right & 0xFF);
            response_buffer[7] = (uint8) (ultra_back >> 8);
            response_buffer[8] = (uint8) (ultra_back & 0xFF);
            return 9;
        case 0x0002 :
            printUART("Read Data 0x0002(ToF) Activated\r\n");
            uint16 tof_value = (uint16) Tof_getDist();
            sprintf(strbuf, "tof : %d \r\n", tof_value);
            printUART(strbuf);
            response_buffer[3] = (uint8) (tof_value >> 8);
            response_buffer[4] = (uint8) (tof_value & 0xFF);
            return 5;
        case 0x0003 :
            printUART("Read Data 0x0003(Encoder) Activated\r\n");
            uint16 enc_left = (uint16) Encoder_Get_Rpm_Left();
            uint16 enc_right = (uint16) Encoder_Get_Rpm_Right();
            response_buffer[3] = (uint8) (enc_left >> 8);
            response_buffer[4] = (uint8) (enc_left & 0xFF);
            response_buffer[5] = (uint8) (enc_right >> 8);
            response_buffer[6] = (uint8) (enc_right & 0xFF);
            return 7;
        case 0x0004 :
            printUART("Read Data 0x0004(LED) Activated\r\n");
            uint8 led_left = (uint8) MODULE_P23.OUT.B.P1;
            uint8 led_right = (uint8) MODULE_P23.OUT.B.P3;
            response_buffer[3] = (uint8) (led_left);
            response_buffer[4] = (uint8) (led_right);
            return 5;
        case 0x0005 :
            printUART("Read Data 0x0005(AEB) Activated\r\n");
            uint16 aeb_offvalue = (uint16) Motor_getAebOffset();
            response_buffer[3] = (uint8) (aeb_offvalue >> 8);
            response_buffer[4] = (uint8) (aeb_offvalue & 0xFF);
            return 5;

        default : //Negative Response
            printUART("Read Data Failed, NRC: 0x31\r\n");
            response_buffer[0] = 0x7F; // Negative Response
            response_buffer[1] = 0x22; // SID
            response_buffer[2] = 0x31; // NRC: requestOutOfRange(DID)
            return 3;
    }

    return 0;
}

/* Write Data by Identifier(0x2E) */
static uint16 WriteDataByIentifier (uint8 *request_uds, uint8 *response_buffer)
{
    uint16 data_id = (request_uds[1] << 8) | request_uds[2];

    response_buffer[0] = 0x6E;
    response_buffer[1] = request_uds[1];
    response_buffer[2] = request_uds[2];

    switch (data_id)
    {
        case 0x1001 : //AEB offset 값 변경
            printUART("Write Data 0x1001(AEB) Activated\r\n");
            uint16 change_value = (request_uds[3] << 8) | request_uds[4];
            Motor_setAebOffset(change_value);
            sprintf(strbuf, "AEB offset : %d\r\n", Motor_getAebOffset());
            printUART(strbuf);
            return 3;
        default :
            printUART("Write Data Failed, NRC: 0x31\r\n");
            response_buffer[0] = 0x7F; // Negative Response
            response_buffer[1] = 0x2E; // SID
            response_buffer[2] = 0x31; // NRC: requestOutOfRange(DID)
            return 3;
    }

    return 0;
}

/* Read DTC Information(0x19) */
static uint16 ReadDTCInformation (uint8 *request_uds, uint8 *response_buffer)
{
    uint8 subfunction = request_uds[1];

    response_buffer[0] = 0x59;          // Positive Response
    response_buffer[1] = subfunction;   // Sub-function

    switch (subfunction)
    {
        case 0x01 : // Report Number of DTC by Status Mask
            printUART("Read DTC count(0x01) Activated\r\n");
            uint16 dtc_count = DTC_Manager_Count();
            sprintf(strbuf, "cnt : %d\r\n", dtc_count);
            printUART(strbuf);
            response_buffer[2] = request_uds[2]; // DTC Status Availability Mask (Check All)
            response_buffer[3] = 0x01;           // DTC Format Identifier
            response_buffer[4] = (dtc_count >> 8);
            response_buffer[5] = (dtc_count & 0xFF);
            return 6;
        case 0x02 : // Report DTC by Status Mask
            printUART("Read DTC list(0x02) Activated\r\n");
            response_buffer[2] = request_uds[2];
            uint8 buffer_idx = 3;
            for (int idx = 0; idx < MAX_SUPPORTED_DTCS; idx++)
            {
                uint32 dtc_record = DTC_Manager_Check(idx);
                sprintf(strbuf, "dtc : %x\r\n", dtc_record);
                printUART(strbuf);
                if (dtc_record != 0x00000000)
                {
                    response_buffer[buffer_idx++] = (dtc_record >> 24);
                    response_buffer[buffer_idx++] = (dtc_record >> 16);
                    response_buffer[buffer_idx++] = (dtc_record >> 8);
                    response_buffer[buffer_idx++] = (dtc_record & 0xFF);
                }
            }
            return buffer_idx;
        default :
            printUART("Read DTC Information Failed, NRC: 0x12\r\n");
            response_buffer[0] = 0x7F; // Negative Response
            response_buffer[1] = 0x19; // SID
            response_buffer[2] = 0x12; // NRC: requestOutOfRange(sub-function)
            return 3;
    }

    return 0;
}

/* Clear Diagnostic Information(0x14) */
static uint16 ClearDiagnosticInformation (uint8 *request_uds, uint8 *response_buffer)
{
    uint32 groupofDTC = (request_uds[1] << 16) | (request_uds[2] << 8) | request_uds[3];
    if(groupofDTC == 0xFFFFFF)
    {
        printUART("Clear DTC Information(0xFFFFFF) Activated\r\n");
        DTC_Manager_Clear();
        response_buffer[0] = 0x54;
        return 1;
    }
    else
    {
        printUART("Clear DTC Information Failed, NRC: 0x31\r\n");
        response_buffer[0] = 0x7F; // Negative Response
        response_buffer[1] = 0x14; // SID
        response_buffer[2] = 0x31; // NRC: requestOutofRange(groupofDTC)
        return 3;
    }
}

/* Control DTC Setting(0x85) */
static uint16 ControlDTCSetting (uint8 *request_uds, uint8 *response_buffer)
{
    uint8 DTCSettingType = request_uds[1];
    response_buffer[0] = 0xC5;

    switch(DTCSettingType)
    {
        case 0x01:
            printUART("DTC Setting ON(0x01) Activated\r\n");
            DTC_Manager_Setting(1);
            response_buffer[1] = 0x01;
            return 2;
        case 0x02:
            printUART("DTC Setting OFF(0x02) Activated\r\n");
            DTC_Manager_Setting(0);
            response_buffer[1] = 0x02;
            return 2;
        default:
            printUART("DTC Setting Modification Failed, NRC: 0x12\r\n");
            response_buffer[0] = 0x7F; // Negative Response
            response_buffer[1] = 0x85; // SID
            response_buffer[2] = 0x12; // NRC: requestOutOfRange(sub-function)
            return 3;
    }
}

IFX_INTERRUPT(Uds_Stm2_Cmp0_Isr_Handler, 0, ISR_PRIORITY_STM2_CMP0);
void Uds_Stm2_Cmp0_Isr_Handler(void) {
    MODULE_STM2.CMP[0].U = (unsigned int)((MODULE_STM2.TIM0.U | ((uint64)MODULE_STM2.CAP.U << 32)) + 100000*CPU_CLOCK_MHZ);

    DTC_Manager_Update();

    if(Current_Session == EXTENDED_SESSION)
    {
        Session_Time_Out_Cnt++;
        if(Session_Time_Out_Cnt == 50) //5 second
        {
            printUART("Session Time-Out! Return to DEFAULT Session\r\n");
            Session_Time_Out_Cnt = 0;
            Current_Session = DEFAULT_SESSION;
        }
    }

}
