#include "main.h"
#include "Sota.h"

void main (void)
{
    SYSTEM_Init();

    IfxPort_setPinMode(&MODULE_P00, 5, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(&MODULE_P00, 6, IfxPort_Mode_outputPushPullGeneral);
    MODULE_P00.OUT.B.P5 = 1;
    MODULE_P00.OUT.B.P6 = 1;

    MODULE_P23.IOCR0.B.PC1 = 0x10;
    MODULE_P23.IOCR0.B.PC3 = 0x10;
    MODULE_P23.OUT.B.P1 = 0;
    MODULE_P23.OUT.B.P3 = 0;

    /* Define a MAC Address */
    eth_addr_t ethAddr = {
            .addr[0] = 0x00,
            .addr[1] = 0x00,
            .addr[2] = 0x0c,
            .addr[3] = 0x11,
            .addr[4] = 0x11,
            .addr[5] = 0x11   };

    initLwip(ethAddr); /* Initialize LwIP with the MAC address */

    // UdpEchoInit();
    // TcpEchoInit();
    // DoIP_Init();
    SOMEIP_Init();
    SOMEIPSD_Init();
    sota_server_init();

    while (1)
    {
        Ifx_Lwip_pollTimerFlags(); /* Poll LwIP timers and trigger protocols execution if required */
        Ifx_Lwip_pollReceiveFlags(); /* Receive data package through ETH */
        sota_server_poll();
    } /* End of while */
}
