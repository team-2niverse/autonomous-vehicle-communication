#include "Sys.h"

void Sys_moduleInit(void) {
    Can_Init(BD_500K, CAN_NODE0);
    Encoder_Init();
    Led_Init();
    Motor_init();
    Ultrasonic_Init();

    Geth_init();

    /* Define a MAC Address */
    eth_addr_t ethAddr;
    ethAddr.addr[0] = 0xDE;
    ethAddr.addr[1] = 0xAD;
    ethAddr.addr[2] = 0xBE;
    ethAddr.addr[3] = 0xEF;
    ethAddr.addr[4] = 0xFE;
    ethAddr.addr[5] = 0xED;

    ip_addr_t ipAddr;
    IP4_ADDR(&ipAddr, 192, 168, 2, 20);

    Ifx_Lwip_init(ethAddr, ipAddr);                                 /* Initialize LwIP with the MAC address */

    SOMEIP_Init();
    DoIP_Init();
    Uds_Init();
    Sota_server_init();

}

void Sys_init(void) {
    /* Enable the global interrupts of this CPU */
    IfxCpu_enableInterrupts();

    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    Sys_moduleInit();
}
