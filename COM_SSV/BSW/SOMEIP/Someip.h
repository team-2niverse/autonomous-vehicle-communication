#ifndef BSW_SOMEIP_SOMEIP_H_
#define BSW_SOMEIP_SOMEIP_H_

#include "lwip/ip_addr.h"
#include "Platform_Types.h"

typedef struct {
    uint16 group_id; //서비스그룹id
    uint8 isSub; //구독여부
    uint8 timer; //timer event 여부
    ip_addr_t addr; //목적지 ip
    uint16 port; //목적지 port
} Subscriber;

//서비스랑 메서드의 정의는 있는데 누가(ip,port)하는지는 몰라서 SOMEIP-SD로 찾는것.
//service 설계할때 arxml에서 전체 서비스 리스트랑, 메서드 리스트 있어서 알고 있다고 가정.
typedef struct {
    uint16 service_id;
    uint16 port;
} Service; //ip는 사설 ip 제공


void SOMEIPSD_Init(void);
void SOMEIP_Init(void);
void SOMEIPSD_SendSubEvtGrpAck(unsigned char ip_a, unsigned char ip_b, unsigned char ip_c, unsigned char ip_d);
void SOMEIPSD_SendOfferService(unsigned char ip_a, unsigned char ip_b, unsigned char ip_c, unsigned char ip_d, int i);

int SOMEIP_CheckSubscribers(int i);
void SOMEIP_SendEvent(int i);


#endif /* BSW_SOMEIP_SOMEIP_H_ */
