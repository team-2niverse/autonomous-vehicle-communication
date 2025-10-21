# 게이트웨이(라즈베리 파이 4) 담당
# 진단기(PC)와 통신할 때는 서버를 담당
# ECU(TC375 MCU)와 통신할 때는 클라이언트를 담당

# -----게이트웨이의 주요 기능-----
# 1. Routing Activation 처리
# 2. UDS 메시지 양방향 전달
# 3. ECU와 연결이 끊겼을 때 NRC 0x22 생성 및 PC로 전송

# 진단기는 하나의 UDS 요청에 대한 응답이 돌아올 때까지 다른 요청을 보내지 않음
# → 멀티스레딩 구조를 취하지 않아도 됨 (블로킹이 발생하지 않음)
# → 하나의 스레드로 처리함

import socket
import time
import struct

# ---IP 및 PORT 설정---
PI_SERVER_HOST = '0.0.0.0' # 모든 IP에서의 접속을 허용
PI_SERVER_PORT = 13400
TC375_SERVER_HOST = '192.168.2.20' # ECU(TC375 MCU)의 IP 주소
TC375_SERVER_PORT = 13400

# ---DoIP 및 UDS 메시지---
REP_ROUTING = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x00, 0x06,              # 페이로드 타입: Response Routing Activation
    0x00, 0x00, 0x00, 0x09,  # 페이로드 길이: 9

    # --- 페이로드 ---
    0x0E, 0x00,              # 진단기의 논리 주소 (요청했던 주소)
    0x20, 0x00,              # ECU의 논리 주소 (앞으로 통신할 ECU의 논리 주소)
    0x10,                    # 응답 코드: Successfully activated(10)
    0x00, 0x00, 0x00, 0x00   # 예약 필드(0000)
])

def create_nrc_22_response(original_request):
    # ECU와 연결을 실패할 경우 NRC 0x22 응답을 생성
    # ECU와 게이트웨이의 연결이 끊어진 경우
    pc_addr = original_request[8:10]
    ecu_addr = original_request[10:12]
    original_sid = original_request[12]

    # UDS Payload: SID(78) + original_SID() + NRC(22)
    uds_nrc_payload = bytes([0x7F, original_sid, 0x22])
    # DoIP payload: source addr(ecu) + target addr(pc) + uds payload
    doip_payload = ecu_addr + pc_addr + uds_nrc_payload

    doip_header = bytes([
        0x02, 0xfd, 0x80, 0x01
    ]) + struct.pack('>I', len(doip_payload))

    return doip_header + doip_payload


# ----- PC-ECU 세션 처리 함수 -----
def handle_session(pc_socket):
    # PC 클라이언트와 전체 진단 게이트웨이를 처리하는 함수
    # pc_socket: PC와 연결된 소켓 객체
    ecu_socket = None # ECU 서버에 접속하는 클라이언트 소켓
    is_ecu_connected = None # ECU와의 연결 상태를 저장하는 변수

    try:
        # --- Routing Activation ---
        print("Routing Activation 요청을 기다립니다...")
        pc_socket.settimeout(10.0) # 서버 소켓의 타임아웃 설정 (무한 대기 방지)

        try:
            routing_req = pc_socket.recv(1024)
            # Routing Activation 요청이 발생하기 전 PC가 연결을 끊은 경우
            if not routing_req:
                print("PC가 연결을 끊었습니다. 세션을 종료합니다.")
                return
        # Routing Activation 요청이 오지 않음 (Timeout)
        except socket.timeout:
            print("Timeout: Routing Activation 요청이 없어 세션을 종료합니다.")
        
        print("Routing Activation 성공 응답을 전송합니다.")
        pc_socket.sendall(REP_ROUTING)

        # --- ECU와 연결 ---
        try:
            print(f"TC375 ECU({TC375_SERVER_HOST}:{TC375_SERVER_PORT})에 연결을 시도합니다...")    
            ecu_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # ECU와 통신할 클라이언트 소켓 생성
            ecu_socket.settimeout(2.0)
            ecu_socket.connect((TC375_SERVER_HOST, TC375_SERVER_PORT))
            is_ecu_connected = True
            print ("TC375 ECU에 성공적으로 연결되었습니다.")    
        # ECU 연결에 실패한 경우 (Timeout 포함)
        except Exception as e:
            print(f"TC375 ECU 연결 실패: {e}. NRC 모드로 동작합니다.")
            # is_ecu_connected는 False 유지

        # --- main Loop ---
        pc_socket.settimeout(None) # PC와 연결이 끊기지 않는 한 프로그램 유지
        while True:
            print("\nPC로부터 메시지 대기 중...")
            pc_data = pc_socket.recv(1024)
            if not pc_data:
                print("PC가 연결을 끊었습니다. 세션을 종료합니다.")
                break
            
            print(f"PC -> Gateway: {pc_data.hex(' ')}")

            # --- ECU와 연결이 끊어져있는 경우 ---
            if not is_ecu_connected:
                print("ECU 연결이 끊겨있어 재연결을 시도합니다...")
                try:
                    ecu_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # 재연결을 위해 소켓 재생성
                    ecu_socket.settimeout(2.0)
                    ecu_socket.connect((TC375_SERVER_HOST, TC375_SERVER_PORT))
                    is_ecu_connected = True
                    print ("TC375 ECU에 성공적으로 재연결되었습니다.")
                except Exception as e:
                    print(f"TC375 ECU 재연결 실패: {e}")
                    # 재연결에 실패할 경우 NRC 응답만 보내고 continue (TesterPresent를 보낸 경우 pass)
                    if len(pc_data) > 12 and pc_data[12] != 0x3E:
                        nrc_response = create_nrc_22_response(pc_data)
                        pc_socket.sendall(nrc_response)
                    continue # 다음 PC 요청을 기다림

            # --- 메시지 중계 ---
            # ECU와 연결이 되어 있는 경우 (is_ecu_connected = True)
            try:
                print("메시지를 ECU로 전달합니다...")
                ecu_socket.sendall(pc_data)

                is_tester_present = len(pc_data) > 12 and pc_data[12] == 0x3E
                if not is_tester_present:
                    print("ECU로부터 응답 대기 중...")
                    ecu_response = ecu_socket.recv(1024)

                    # 응답이 정상적으로 왔을 때
                    if ecu_response:
                        print(f"ECU -> Gateway: {ecu_response.hex(' ')}")
                        print("ECU 응답을 PC로 전달합니다...")
                        pc_socket.sendall(ecu_response)
                    # 응답이 비어있을 때 (ECU가 정상 종료)
                    else:
                        print("ECU가 연결을 종료했습니다. NRC 모드로 전환합니다.")
                        is_ecu_connected = False
                        ecu_socket.close()
                        nrc_response = create_nrc_22_response(pc_data)
                        pc_socket.sendall(nrc_response)

            except socket.timeout as e:
                print(f"ECU가 연결을 종료했습니다({e}). NRC 모드로 전환합니다.")
                is_ecu_connected = False
                if ecu_socket:
                    ecu_socket.close()
                
                if not (len(pc_data) > 12 and pc_data[12] == 0x3E):
                    nrc_response = create_nrc_22_response(pc_data)
                    pc_socket.sendall(nrc_response)

    except Exception as e:
        print(f"세션 처리 중 예상치 못한 오류 발생: {e}")
    finally:
        print("세션을 종료하고 ECU 소켓을 닫습니다.")
        if ecu_socket:
            ecu_socket.close()


# ----- 메인 프로그램 -----
def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # TCP/IP 서버 소켓 생성
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # 소켓의 세부 옵션 설정 (이전 주소를 재사용 + 서버를 껐다 켰다할 때 주소 오류를 방지)
    server_socket.bind((PI_SERVER_HOST, PI_SERVER_PORT)) # IP주소와 포트 번호를 부여 (서버)
    server_socket.listen() # 소켓을 연결 대기 상태로 전환
    print(f"DoIP 게이트웨어 서버가 {PI_SERVER_HOST} : {PI_SERVER_PORT}에서 시작되었습니다.")

    while True:
        try:
            print("\nPC 진단기의 접속을 기다립니다...")
            # 클라이언트(PC)의 접속을 기다림
            # 클라이언트가 접속할 경우 새로운 소켓과 주소 반환 (클라이언트 전용 소켓)
            pc_conn, pc_addr = server_socket.accept()

            # with 구문이 끝나면 pc_conn.close()를 자동으로 호출
            with pc_conn:
                print(f"PC 진단기 접속: {pc_addr}")
                handle_session(pc_conn)

        except KeyboardInterrupt:
            print("\nCtrl+C 입력 감지: 게이트웨이 서버를 종료합니다.")
            break
        except Exception as e:
            print(f"\n메인 서버 루프에서 오류 발생: {e}")
            time.sleep(1) # 잠시 대기 후 while문 재실행
    
    server_socket.close()
    print("게이트웨이 서버가 종료되었습니다.")


# ----- 메인 실행 -----
if __name__ == "__main__":
    main()