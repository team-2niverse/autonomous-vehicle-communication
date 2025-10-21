#ifndef BSW_UDS_DIAGNOSTIC_H_
#define BSW_UDS_DIAGNOSTIC_H_

#include "Ifx_Types.h"

// ECU가 관리하는 DTC 최대 개수
#define MAX_SUPPORTED_DTCS 6

typedef struct
{
    uint32 dtc_id;     // DTC 고유 번호 (3byte)
    uint8 status_byte; // DTC 상태 (1byte)
} DTC_Info_t;


// DTC HEX NUMBER
// Ultrasonic
#define DTC_ULTRASONIC_LEFT_FAULT   0x9A1113 // B1A11 - Circuit Malfunction
#define DTC_ULTRASONIC_RIGHT_FAULT  0x9A1213 // B1A12 - Circuit Malfunction
#define DTC_ULTRASONIC_BACK_FAULT   0x9A1313 // B1A13 - Circuit Malfunction

// ToF
#define DTC_TOF_FAULT       0x5B3113         // C1B31 - Circuit Malfunction

// Encoder
#define DTC_ENCODER_LEFT_FAULT      0x403513 // C0035 - Circuit Malfunction
#define DTC_ENCODER_RIGHT_FAULT     0x404013 // C0040 - Circuit Malfunction


void DTC_Manager_Init (void);
void DTC_Manager_Setting (int OnOff);
void DTC_Manager_Clear (void);
void DTC_Manager_Update (void);
uint16 DTC_Manager_Count (void);
uint32 DTC_Manager_Check (int idx);

void Encoder_Fault_Left_Init(void);
void Encoder_Fault_Right_Init(void);
void Encoder_Fault_Left_Inc(void);
void Encoder_Fault_Right_Inc(void);

int Check_Ultrasonic_Left_Fault (void);
int Check_Ultrasonic_Right_Fault (void);
int Check_Ultrasonic_Back_Fault (void);
int Check_ToF_Fault (void);
int Check_Encoder_Left_Fault (void);
int Check_Encoder_Right_Fault (void);

#endif /* BSW_UDS_DIAGNOSTIC_H_ */
