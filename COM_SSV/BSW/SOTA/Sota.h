#ifndef BSW_SOTA_SOTA_H_
#define BSW_SOTA_SOTA_H_

#include "lwip/tcp.h"

/**
 * @brief 리프로그래밍 세션의 상태 정의
 */
typedef enum
{
    SS_IDLE,          /**< "START" 명령 대기 상태 */
    SS_RECEIVING_HEX, /**< 데이터(0x00), 주소(0x04), EOF(0x01) 라인 수신 대기 상태 */
    SS_WAIT_ERASE,    /**< 0x04 라인 수신 후, 섹터 지우기 완료 대기 (main 루프가 폴링) */
    SS_CLOSING        /**< 세션 종료 처리 중 상태 */
} SotaStates;

/**
 * @brief 단일 클라이언트와의 리프로그래밍 세션 정보를 담는 구조체
 */
typedef struct
{
    SotaStates state;          /**< 현재 세션 상태 (ReprogStates) */
    struct tcp_pcb *pcb;       /**< 이 세션을 위한 TCP 프로토콜 제어 블록 (PCB) */
    struct pbuf *p;            /**< 아직 처리되지 않은 수신 패킷 버퍼 체인 (pbuf) */
    char line_buffer[256];     /**< TCP 스트림에서 HEX 라인 한 줄을 재조립하기 위한 버퍼 */
    uint16 line_buffer_pos;    /**< line_buffer의 현재 쓰기 위치(인덱스) */
    uint32 upper_address;      /**< HEX 파일의 확장 선형 주소 (레코드 타입 0x04) */
} SotaSession;

void Sota_server_init(void);
void Sota_server_poll(void);

#endif /* BSW_SOTA_SOTA_H_ */
