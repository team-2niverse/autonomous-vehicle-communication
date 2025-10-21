#ifndef BSW_UDS_UDS_H_
#define BSW_UDS_UDS_H_

#include "Platform_Types.h"

#define MAX_SUPPORTED_SIDS 7
// Session State
typedef enum
{
    DEFAULT_SESSION = 0x01,
    EXTENDED_SESSION = 0x03
} UDS_Session_t;

void Uds_Init(void);
void UDS_Stm1_Init_100ms (void);
//void Uds_Stm1_Isr_Handler (void);

uint16 UDS_HandleRequest (struct pbuf *request_pbuf, uint8 *response_buffer);

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


#endif /* BSW_UDS_UDS_H_ */
