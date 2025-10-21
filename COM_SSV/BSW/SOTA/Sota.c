#include "sota.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "IfxFlash.h"
#include "IfxCpu.h"
#include "UART_Logging.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define SOTA_TCP_PORT 13401 // 리프로그래밍에 사용할 TCP 포트

#define DATA_FLASH_0                IfxFlash_FlashType_D0       /* Define the Data Flash Bank to be used            */
#define UCB_SWAP_ORIG_ADDR 0xAF402E00
#define UCB_SWAP_COPY_ADDR 0xAF403E00
#define UCB_OTP0_ORIG_ADDR 0xAF404000
#define UCB_OTP0_COPY_ADDR 0xAF405000
#define STD_ADDR_MAP 0x00000055
#define ALT_ADDR_MAP 0x000000AA
#define CONFIRMED_CODE 0x57B5327F

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
// 단일 클라이언트 연결만 지원한다고 가정하고 전역 변수로 세션 관리
SotaSession g_CurrentSession;

/*********************************************************************************************************************/
/*-------------------------------------------Function Prototypes (Static)--------------------------------------------*/
/*********************************************************************************************************************/
static err_t sota_accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t sota_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void sota_error_callback(void *arg, err_t err);
static void sota_close_session(struct tcp_pcb *tpcb, SotaSession *es);
static void sota_unpack_and_process(struct tcp_pcb *tpcb, SotaSession *es);
static void handle_hex_line(struct tcp_pcb *tpcb, SotaSession *es, const char *line);

#define MEM(address)                *((uint32 *)(address))
/*********************************************************************************************************************/
/*------------------------------------------Flash Operation Stubs---------------------------------------------*/
/*********************************************************************************************************************/

void _SOTA_eraseInactivePFbyUpperAddrNonBlock(uint32 upperAddr)
{
    static uint64 erased = 0;

    if ((erased >> (upperAddr - 0xA030)) & 1)
        return;

    /* Get the current password of the Safety WatchDog module */
    uint16 endInitSafetyPassword = IfxScuWdt_getSafetyWatchdogPasswordInline();
    IfxFlash_FlashType pfType = IfxFlash_FlashType_P1;
    if ((MODULE_SCU.STMEM1.U >> 16) & 0x3 == 0x2)
        pfType = IfxFlash_FlashType_P0;

    /* Erase the sector */
    IfxScuWdt_clearSafetyEndinitInline(endInitSafetyPassword);      /* Disable EndInit protection                   */
    IfxFlash_eraseMultipleSectors(upperAddr << 16, 4); /* Erase the given sector                       */
    IfxScuWdt_setSafetyEndinitInline(endInitSafetyPassword);        /* Enable EndInit protection                    */

    /* Wait until the sector is erased */
    // IfxFlash_waitUnbusy(0, pfType);

    erased |= (1 << upperAddr - 0xA030);
}

void _SOTA_writeInactivePFbyPage(uint32 pageAddr, uint32 *word)
{
    uint16 endInitSafetyPassword = IfxScuWdt_getSafetyWatchdogPassword();
    IfxFlash_FlashType pfType = IfxFlash_FlashType_P1;
    if ((MODULE_SCU.STMEM1.U >> 16) & 0x3 == 0x2)
        pfType = IfxFlash_FlashType_P0;

    /* --------------- WRITE PROCESS --------------- */
    /* Enter in page mode */
    IfxFlash_enterPageMode(pageAddr);

    /* Wait until page mode is entered */
    IfxFlash_waitUnbusy(0, pfType);

    /* Write 32 bytes (8 double words) into the assembly buffer */
    for(int i = 0; i < 4; i++)     /* Loop over the page length                */
    {
        IfxFlash_loadPage2X32(pageAddr+i*8, word[i*2], word[i*2+1]); /* Load 2 words of 32 bits each */
    }

    /* Write the page */
    IfxScuWdt_clearSafetyEndinitInline(endInitSafetyPassword);      /* Disable EndInit protection               */
    IfxFlash_writePage(pageAddr);                                   /* Write the page                           */
    IfxScuWdt_setSafetyEndinitInline(endInitSafetyPassword);        /* Enable EndInit protection                */

    /* Wait until the page is written in the Program Flash memory */
    IfxFlash_waitUnbusy(0, pfType);
}

uint32 _SOTA_getRawWord(char *data, int nbytes)
{
    char temp[9] = "00000000";

    for (int i = 0; i < nbytes; i++)
    {
        if (data[i*2] == '\0' || data[i*2+1] == '\0')
            break;
        temp[6-i*2] = data[i*2];
        temp[7-i*2] = data[i*2+1];
    }

    return (uint32)strtoul(temp, NULL, 16);
}

void _SOTA_writeInactivePFbyHexRecord(uint32 byteCnt, uint32 addr, char *data)
{
    static uint32 word[8] = {0, };
    static uint32 loadedPageAddr = 0;
    uint32 pageAddr = addr & 0xFFFFFFE0;
    int idx_word = (addr & 0x0000001F) >> 2;

    if ((loadedPageAddr != 0 && pageAddr != loadedPageAddr) || data == NULL)
    {
        _SOTA_writeInactivePFbyPage(loadedPageAddr, word);
        memset(word, 0, sizeof(uint32) * 8);
        loadedPageAddr = pageAddr;
    }

    if (loadedPageAddr == 0)
        loadedPageAddr = pageAddr;

    for (int i = 0; i < byteCnt; i += 4)
    {
        if (idx_word == 8)
        {
            _SOTA_writeInactivePFbyPage(pageAddr, word);
            memset(word, 0, sizeof(uint32) * 8);
            idx_word = 0;
            loadedPageAddr = pageAddr + 0x20;
        }
        word[idx_word] = _SOTA_getRawWord(&data[i*2], 4);
        idx_word++;
    }
}


void _SOTA_activeUcbSwapEntry(int entryIdx, uint32 addrMap)
{
    uint32 pageAddr = UCB_SWAP_ORIG_ADDR + entryIdx * 0x10;
    uint16 endInitSafetyPassword = IfxScuWdt_getSafetyWatchdogPassword();

    IfxFlash_enterPageMode(pageAddr);
    IfxFlash_waitUnbusy(0, DATA_FLASH_0);
    IfxFlash_loadPage2X32(pageAddr, addrMap, pageAddr);      /* Load two words of 32 bits each            */
    IfxScuWdt_clearSafetyEndinit(endInitSafetyPassword);     /* Disable EndInit protection                */
    IfxFlash_writePage(pageAddr);                            /* Write the page                            */
    IfxScuWdt_setSafetyEndinit(endInitSafetyPassword);       /* Enable EndInit protection                 */
    IfxFlash_waitUnbusy(0, DATA_FLASH_0);

    pageAddr += IFXFLASH_DFLASH_PAGE_LENGTH;

    IfxFlash_enterPageMode(pageAddr);
    IfxFlash_waitUnbusy(0, DATA_FLASH_0);
    IfxFlash_loadPage2X32(pageAddr, CONFIRMED_CODE, pageAddr);   /* Load two words of 32 bits each            */
    IfxScuWdt_clearSafetyEndinit(endInitSafetyPassword);     /* Disable EndInit protection                */
    IfxFlash_writePage(pageAddr);                            /* Write the page                            */
    IfxScuWdt_setSafetyEndinit(endInitSafetyPassword);       /* Enable EndInit protection                 */
    IfxFlash_waitUnbusy(0, DATA_FLASH_0);
}

int SOTA_swapAB()
{
    uint32 ucbSwapNowEntryIdx = ((MODULE_SCU.STMEM1.U >> 19) & 0x1F) >> 1;
    uint32 ucbSwapNextEntryIdx = ucbSwapNowEntryIdx + 1;
    uint32 nowAddrMap = MEM(UCB_SWAP_ORIG_ADDR + ucbSwapNowEntryIdx * 0x10);

    if (ucbSwapNextEntryIdx >= 16)
    {
        uint16 endInitSafetyPassword = IfxScuWdt_getSafetyWatchdogPassword();

        /* Erase the sector */
        IfxScuWdt_clearSafetyEndinit(endInitSafetyPassword);        /* Disable EndInit protection                       */
        IfxFlash_eraseSector(UCB_SWAP_ORIG_ADDR);                   /* Erase the given sector           */
        IfxScuWdt_setSafetyEndinit(endInitSafetyPassword);          /* Enable EndInit protection                        */
        IfxFlash_waitUnbusy(0, DATA_FLASH_0);

        /* confirmation UNLOCK */
        IfxFlash_enterPageMode(0xAF402FF0);
        IfxFlash_waitUnbusy(0, DATA_FLASH_0);
        IfxFlash_loadPage2X32(0xAF402FF0, 0x43211234, 0x00000000);   /* Load two words of 32 bits each            */
        IfxScuWdt_clearSafetyEndinit(endInitSafetyPassword);     /* Disable EndInit protection                */
        IfxFlash_writePage(0xAF402FF0);                            /* Write the page                            */
        IfxScuWdt_setSafetyEndinit(endInitSafetyPassword);       /* Enable EndInit protection                 */
        IfxFlash_waitUnbusy(0, DATA_FLASH_0);

        _SOTA_activeUcbSwapEntry(0, nowAddrMap);

        ucbSwapNowEntryIdx = 0;
        ucbSwapNextEntryIdx = 1;
    }

    // _SOTA_activeUcbSwapEntry(ucbSwapNextEntryIdx, STD_ADDR_MAP);
    if (nowAddrMap == ALT_ADDR_MAP)
        _SOTA_activeUcbSwapEntry(ucbSwapNextEntryIdx, STD_ADDR_MAP);
    else
        _SOTA_activeUcbSwapEntry(ucbSwapNextEntryIdx, ALT_ADDR_MAP);

    // overwrite
    uint32 ucbSwapNowEntryAddr = UCB_SWAP_ORIG_ADDR + ucbSwapNowEntryIdx * 0x10;
    uint16 endInitSafetyPassword = IfxScuWdt_getSafetyWatchdogPassword();
    IfxFlash_enterPageMode(ucbSwapNowEntryAddr + 0x8);
    IfxFlash_waitUnbusy(0, DATA_FLASH_0);
    IfxFlash_loadPage2X32(ucbSwapNowEntryAddr + 0x8, 0xFFFFFFFF, ucbSwapNowEntryAddr + 0x8);
    IfxScuWdt_clearSafetyEndinit(endInitSafetyPassword);
    IfxFlash_writePage(ucbSwapNowEntryAddr + 0x8);
    IfxScuWdt_setSafetyEndinit(endInitSafetyPassword);
    IfxFlash_waitUnbusy(0, DATA_FLASH_0);

    return 0;
}


/*********************************************************************************************************************/
/*---------------------------------------------LwIP Callback Functions-----------------------------------------------*/
/*********************************************************************************************************************/

/**
 * @brief 새 클라이언트 연결 요청을 수락했을 때 호출되는 콜백
 */
static err_t sota_accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);

    printUART("New client connected.\r\n");

    // 세션 초기화
    SotaSession *ss = &g_CurrentSession;
    ss->state = SS_IDLE;
    ss->pcb = newpcb;
    ss->p = NULL;
    ss->line_buffer_pos = 0;
    ss->upper_address = 0;

    // LwIP에 이 세션 구조체를 모든 콜백의 첫번째 인자로 넘기도록 등록
    tcp_arg(newpcb, ss);
    tcp_recv(newpcb, sota_recv_callback);
    tcp_err(newpcb, sota_error_callback);
    // tcp_poll, tcp_sent 는 이 예제에서 필수는 아님

    // 클라이언트에게 환영 메시지 전송
    const char* welcome_msg = "Welcome! Send 'START' to begin reprogramming.\r\n";
    tcp_write(newpcb, welcome_msg, strlen(welcome_msg), TCP_WRITE_FLAG_COPY);
    tcp_output(newpcb);

    return ERR_OK;
}

/**
 * @brief 클라이언트로부터 데이터 수신 시 호출되는 콜백
 */
static err_t sota_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    SotaSession *ss = (SotaSession*)arg;

    // p가 NULL이면 클라이언트가 연결을 종료했다는 의미
    if (p == NULL)
    {
        printUART("Client disconnected.\r\n");
        sota_close_session(tpcb, ss);
        return ERR_OK;
    }

    // 에러 처리
    if (err != ERR_OK)
    {
        pbuf_free(p);
        return err;
    }

    // 수신된 pbuf를 세션의 pbuf 체인에 연결
    if (ss->p == NULL)
    {
        ss->p = p;
    }
    else
    {
        pbuf_chain(ss->p, p);
    }

    // 수신된 데이터 처리 시도
    sota_unpack_and_process(tpcb, ss);

    return ERR_OK;
}

/**
 * @brief TCP 에러 발생 시 호출되는 콜백
 */
static void sota_error_callback(void *arg, err_t err)
{
    LWIP_UNUSED_ARG(err);
    SotaSession *ss = (SotaSession*)arg;
    printUART("TCP Error occurred. Closing session.\r\n");
    if (ss != NULL)
    {
        // 간단히 세션 상태만 초기화 (복잡한 메모리 해제는 없으므로)
        ss->state = SS_IDLE;
        ss->pcb = NULL;
    }
}

/*********************************************************************************************************************/
/*-------------------------------------------------Protocol Logic----------------------------------------------------*/
/*********************************************************************************************************************/

/**
 * @brief pbuf 체인을 순회하며 데이터를 line_buffer로 재조립하고, 라인 완성 시 핸들러 호출
 */
static void sota_unpack_and_process(struct tcp_pcb *tpcb, SotaSession *ss)
{
    // 지우기 작업 완료를 기다리는 중이면, 새 데이터를 처리하지 않음
    if (ss->state == SS_WAIT_ERASE)
    {
        return;
    }

    struct pbuf *ptr;
    while (ss->p != NULL)
    {
        ptr = ss->p;
        uint16_t i;
        for (i = 0; i < ptr->len; i++)
        {
            char c = ((char*)ptr->payload)[i];

            if (ss->line_buffer_pos < sizeof(ss->line_buffer) - 1)
            {
                ss->line_buffer[ss->line_buffer_pos++] = c;
            }

            if (c == '\n')
            {
                ss->line_buffer[ss->line_buffer_pos - 1] = '\0'; // 문자열 종료
                handle_hex_line(tpcb, ss, ss->line_buffer);
                ss->line_buffer_pos = 0; // 라인 버퍼 리셋

                // handle_hex_line이 상태를 WAIT_ERASE로 바꿨다면 즉시 처리 중단
                if (ss->state == SS_WAIT_ERASE)
                {
                    break;
                }
            }
        }

        // 실제 처리한 바이트만큼 TCP 수신 윈도우를 열어줌
        uint16_t bytes_processed = i;
        tcp_recved(tpcb, bytes_processed);

        if (bytes_processed == ptr->len)
        {
            // pbuf 하나를 다 처리함
            ss->p = ptr->next;
            if (ss->p) pbuf_ref(ss->p);
            pbuf_free(ptr);
        }
        else
        {
            // pbuf 중간에서 멈춤 (state가 WAIT로 바뀜)
            pbuf_header(ptr, -(s16_t)bytes_processed);
            break; // pbuf 처리 루프 중단
        }
    }
}

/**
 * @brief 완성된 HEX 라인 한 줄을 파싱하고, 상태에 따라 적절한 작업을 수행
 */
static void handle_hex_line(struct tcp_pcb *tpcb, SotaSession *ss, const char *line)
{
    // (1) "START" 명령 처리
    // strncmp를 사용하여 개행 문자 등을 무시하고 비교
    if (ss->state == SS_IDLE && strncmp(line, "START", 5) == 0)
    {
        ss->state = SS_RECEIVING_HEX;
        printUART("Received START command. State -> RECEIVING_HEX\r\n");
        const char* msg = "ACK: START OK. Send HEX.\r\n";
        tcp_write(tpcb, msg, strlen(msg), TCP_WRITE_FLAG_COPY);
        tcp_output(tpcb);
        return;
    }

    // (2) 데이터/주소/EOF 라인 처리
    if (ss->state == SS_RECEIVING_HEX)
    {
        if (line[0] != ':') return; // HEX 라인이 아니면 무시

        unsigned int byte_count, lower_address, record_type;
        char data[65] = {0, };
        sscanf(&line[1], "%2x%4x%2x", &byte_count, &lower_address, &record_type);
        strncpy(data, &line[9], byte_count*2);

        switch (record_type)
        {
            case 0x00: // Data Record
            {
                printUART("PFLASH write\r\n");

                if (ss->upper_address > 0)
                    _SOTA_writeInactivePFbyHexRecord(byte_count, ((uint32)(ss->upper_address) << 16) | lower_address, data);

                const char* msg = "ACK: Line OK.\r\n";
                tcp_write(tpcb, msg, strlen(msg), TCP_WRITE_FLAG_COPY);
                tcp_output(tpcb);
                break;
            }
            case 0x01: // End of File Record
            {
                printUART("Received EOF Record. Finalizing session.\r\n");

                _SOTA_writeInactivePFbyHexRecord(0, 0, NULL);
                SOTA_swapAB();
                MODULE_P00.OUT.B.P6 = 0;
                const char* msg = "ACK: File received. SOTA complete.\r\n";
                tcp_write(tpcb, msg, strlen(msg), TCP_WRITE_FLAG_COPY);
                tcp_output(tpcb);
                // sota_close_session(tpcb, ss);
                break;
            }
            case 0x04: // Extended Linear Address Record
            {
                ss->upper_address = (uint32)strtoul(data, NULL, 16);
                if (0x8000 <= ss->upper_address && ss->upper_address < 0x8030)
                    ss->upper_address += 0x2000;
                else if (ss->upper_address < 0xA000 || ss->upper_address >= 0xA030)
                {
                    ss->upper_address = 0;
                    return;          // not flash area
                }
                ss->upper_address += 0x0030;

                printUART("Received Address Record (0x04). State -> WAIT_ERASE\r\n");
                _SOTA_eraseInactivePFbyUpperAddrNonBlock(ss->upper_address);
                ss->state = SS_WAIT_ERASE;
                // ACK는 main 루프가 지우기 완료 후 보냄
                break;
            }
            default: // 그 외 레코드 타입
            {
                printUART("Unknown line\r\n");
                const char* msg = "ACK: Line OK. (Ignored)\r\n";
                tcp_write(tpcb, msg, strlen(msg), TCP_WRITE_FLAG_COPY);
                tcp_output(tpcb);
                break;
            }
        }
    }
}

/**
 * @brief TCP 세션을 종료하고 리소스를 정리
 */
static void sota_close_session(struct tcp_pcb *tpcb, SotaSession *ss)
{
    // 콜백 함수들 제거
    tcp_arg(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);
    tcp_sent(tpcb, NULL);

    // pbuf 체인이 남아있으면 해제
    if (ss->p != NULL)
    {
        pbuf_free(ss->p);
        ss->p = NULL;
    }

    // TCP 연결 종료
    tcp_close(tpcb);

    // 전역 세션 상태 초기화
    ss->state = SS_IDLE;
    ss->pcb = NULL;
    ss->line_buffer_pos = 0;
    ss->upper_address = 0;
}


/*********************************************************************************************************************/
/*-------------------------------------------Public Function Implementations-----------------------------------------*/
/*********************************************************************************************************************/

/**
 * @brief 리프로그래밍 TCP 서버를 초기화하고 리스닝을 시작합니다.
 */
void Sota_server_init(void)
{
    struct tcp_pcb *pcb;
    printUART("Initializing Sota Server...\r\n");

    pcb = tcp_new();
    if (pcb == NULL)
    {
        printUART("Error creating PCB. Out of memory?\r\n");
        return;
    }

    err_t err = tcp_bind(pcb, IP_ADDR_ANY, SOTA_TCP_PORT);
    if (err != ERR_OK)
    {
        printUART("Error binding PCB\r\n");
        return;
    }

    pcb = tcp_listen(pcb);
    tcp_accept(pcb, sota_accept_callback);

    printUART("Server listening\r\n");
}

/**
 * @brief main 루프에서 주기적으로 호출되어야 하는 함수입니다.
 */
void Sota_server_poll(void)
{
    SotaSession *ss = &g_CurrentSession;

    // 연결된 클라이언트가 있고, 지우기 완료를 기다리는 상태일 때만 확인
    if (ss->pcb != NULL && ss->state == SS_WAIT_ERASE)
    {
        unsigned char flashBusy = ((MODULE_SCU.STMEM1.U >> 16) & 0x3 == 0x2) ? DMU_HF_STATUS.B.P0BUSY : DMU_HF_STATUS.B.P1BUSY;
        if (!flashBusy)
        {
            // 상태를 다시 '수신 중'으로 변경
            ss->state = SS_RECEIVING_HEX;

            // 0x04 라인에 대한 "지연된 ACK"를 여기서 전송
            printUART("Erase done. State -> RECEIVING_HEX. Sending delayed ACK.\r\n");
            const char* msg = "ACK: Line OK.\r\n";
            tcp_write(ss->pcb, msg, strlen(msg), TCP_WRITE_FLAG_COPY);
            tcp_output(ss->pcb);

            // 지우는 동안 쌓였을지 모르는 pbuf 처리를 위해 수동 호출
            sota_unpack_and_process(ss->pcb, ss);
        }
    }
}
