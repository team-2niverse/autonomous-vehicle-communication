//#include <Drivers/include/can_bsp.h>
//#include <Drivers/include/motor_bsp.h>
#include "Drivers.h"
#include "App/include/can_task.h"
#include "App/include/parking_task.h" // 주차 태스크 세마포어 사용을 위해 추가

// CAN 메시지 큐 생성
QueueHandle_t xCanRxQueue;

void vCanMessageHandlerTask(void *pvParameters)
{
    CanMessage_t xReceivedMessage;

    // CAN 메시지 큐 생성 (10개 메시지 저장 가능)
    xCanRxQueue = xQueueCreate(10, sizeof(CanMessage_t));

    if (xCanRxQueue == NULL)
    {
        // 큐 생성 실패 처리 (예: 오류 LED 켜기)
        while(1);
    }

    while (1)
    {
        // 큐에서 CAN 메시지가 수신될 때까지 무한정 대기
        if (xQueueReceive(xCanRxQueue, &xReceivedMessage, portMAX_DELAY) == pdPASS)
        {
            my_printf("CAN MSG Received! ID: %d\n", xReceivedMessage.id);
            // 수신된 메시지 ID에 따라 처리
            if (xReceivedMessage.id == 0x100) // 원격 주행 명령
            {
                my_printf("remote controlling...\n");
                // 주차 모드 중이 아닐 때만 모터 제어
                if (Can_Get_Parking() == 0) // Can_Get_Parking()은 Bsp/can_bsp.h에 정의되어야 함
                {
                    Motor_Set_Left(xReceivedMessage.data[0], xReceivedMessage.data[1]);
                    Motor_Set_Right(xReceivedMessage.data[2], xReceivedMessage.data[3]);
//                    my_printf("remote controlling...");
                }
            }
            else if (xReceivedMessage.id == 0x102) // 주기적 상태 업데이트
            {
                my_printf("ID 0x102 Data: Park=%d, LeftSig=%d, RightSig=%d\n", xReceivedMessage.data[2], xReceivedMessage.data[4], xReceivedMessage.data[5]);
                // 이전 메시지의 상태를 기억하기 위한 static 변수
                static uint8_t prev_data[8] = {0};

                // --- 주차 로직 (상태가 0 -> 1 로 바뀔 때만 시작) ---
                if (xReceivedMessage.data[2] == 1 && prev_data[2] == 0)
                {
                    my_printf("Parking command received. Starting parking task...\n");
                    Can_Let_Parking(1); // 주차 모드 상태를 '시작'으로 변경
                    xSemaphoreGive(xParkingStartSemaphore); // 주차 태스크에 시작 신호 전송
                }
                else if (xReceivedMessage.data[2] == 0 && prev_data[2] == 1)
                {
                    // 주차 모드가 1->0으로 바뀌었을 때 (예: 원격으로 취소 시)
                    Can_Let_Parking(0);
                    // 여기에 진행중인 주차를 강제로 중단시키는 로직을 추가할 수 있습니다.
                }

                // 현재 상태를 다음 비교를 위해 이전 상태 변수에 복사
                for (int i = 0; i < 8; i++)
                {
                    prev_data[i] = xReceivedMessage.data[i];
                }
            }
            else if (xReceivedMessage.id == 522) // TOF 센서 + AEB
            {
//                my_printf("TOF RAW: 0x%02X 0x%02X\n", xReceivedMessage.data[0], xReceivedMessage.data[1]);

                unsigned int front_dist = (unsigned int)(xReceivedMessage.data[0] | (xReceivedMessage.data[1] << 8));
//                my_printf("front dist: %d mm\n", front_dist);
                Can_Set_Front_Dist(front_dist);
            }
        }
    }
}
