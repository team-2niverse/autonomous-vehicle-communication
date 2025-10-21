# 1. 필요한 도구 상자 가져오기
# 파이썬에서 네트워크 통신(전화 걸기, 데이터 주고받기 등) 기능을 사용하기 위해
# 기본으로 제공되는 'socket' 라이브러리를 불러옵니다.
# time: 프로그램의 실행을 잠시 멈추는(sleep) 등 시간 관련 기능을 사용하기 위해 가져옵니다.
# struct: 바이트(bytes) 덩어리를 정수(integer), 실수(float) 등 의미 있는 데이터 형태로 해석하거나 그 반대로 변환할 때 사용합니다.
import socket
import time
import struct

# --- 통신 설정 ---
# SERVER_HOST: 접속할 상대방, 즉 라즈베리 파이 게이트웨이의 IP 주소(집 주소)를 지정합니다.
SERVER_HOST = '192.168.203.8'
# SERVER_PORT: 라즈베리 파이에서 실행 중인 여러 프로그램 중 DoIP 게이트웨이 프로그램(아파트 호수)을 찾아가기 위한 포트 번호입니다.
# DoIP 표준 포트인 13400을 사용합니다.
SERVER_PORT = 13400

# 2. 미리 정의된 DoIP 요청 메시지
# 서버로 보낼 요청 메시지를 미리 바이트 배열(bytes) 형태로 만들어 둡니다.
# 이렇게 하면 코드가 더 깔끔해지고 재사용하기 좋습니다.

# --- VIN(차대번호) 요청 메시지 (DID: 0xF190) ---
# UDS 표준에 따라 '차대번호를 읽어달라'는 요청을 DoIP 형식으로 포장한 것입니다.
READ_VIN_REQ = bytes([
    0x02, 0xfd,              # 프로토콜 버전 (프로토콜 버전(0x02)과 그 버전의 비트를 뒤집은 값(0xfd). 간단한 오류 검증용입니다.)
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7
    0x0E, 0x00,              # 소스 논리 주소 (진단기) (소스 논리 주소 (Source Logical Address): 보내는 쪽, 즉 진단기(PC)의 주소입니다.)
    0x10, 0x00,              # 타겟 논리 주소 (ECU) (타겟 논리 주소 (Target Logical Address): 받는 쪽, 즉 특정 ECU의 주소입니다.)
    0x22, 0xF1, 0x90         # UDS Payload: SID(22), DID(F190) (UDS Payload: 실제 진단 요청 내용입니다. SID(0x22: ReadDataByIdentifier), DID(0xF190: VIN))
])

# --- 가변저항 값 요청 메시지 (DID: 0x0001) ---
# 'DID 0001에 해당하는 값을 읽어달라'는 요청을 DoIP 형식으로 포장한 것입니다.
READ_VAR_RESISTOR_REQ = bytes([
    0x02, 0xfd,
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x10, 0x00,              # 타겟 논리 주소 (ECU)
    0x22, 0x00, 0x01         # UDS Payload: SID(22), DID(0001)
])

# 3. 메인 통신 로직 실행
# try...except... 구문을 사용하여 통신 중 발생할 수 있는 모든 오류를 처리합니다.
try:
    # 'with' 구문은 소켓을 사용한 후 자동으로 닫아주는(자원 정리) 편리한 기능입니다.
    # socket.socket()으로 TCP/IP 통신을 위한 소켓(전화기)을 생성합니다.
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        print(f"Connecting to {SERVER_HOST}:{SERVER_PORT}...")

        # .connect() 함수로 라즈베리 파이 서버에 실제로 접속(전화 걸기)을 시도합니다.
        s.connect((SERVER_HOST, SERVER_PORT))
        print("Successfully connected!")

        # 1초 동안 잠시 기다립니다. 서버가 클라이언트의 연결을 완전히 수락하고
        # 다음 데이터를 받을 준비를 할 시간을 주기 위함입니다.
        time.sleep(1)

        # --- 가변저항 값 읽기 요청 전송 ---
        print("\nStep 1: Sending Diagnostic Message (Read VIN)...")
        # .sendall() 함수를 이용해 위에서 정의한 가변저항 값 요청 메시지를 서버로 보냅니다.
        s.sendall(READ_VAR_RESISTOR_REQ)
        print("Request sent.")

        # .recv(1024) 함수를 이용해 서버로부터 응답이 올 때까지 기다립니다.
        # 응답이 오면, 최대 1024바이트까지 데이터를 읽어와 response 변수에 저장합니다.
        response = s.recv(1024)
        print(f"Step 2: Received Response (hex): {response.hex()}") # 받은 데이터를 16진수로 보기 좋게 출력

        # --- 수신된 응답 데이터 해석 ---
        # 응답이 유효한지 검사합니다.
        # 1. 응답의 전체 길이가 최소 15바이트 이상인지 확인합니다. (DoIP 헤더 8 + 주소 4 + UDS 헤더 3 = 15)
        # 2. 응답의 UDS 부분이 '긍정 응답(0x62) + 요청했던 DID(0x0001)'인지 확인합니다.
        #    response[12:15]는 12번째부터 14번째 바이트까지를 의미하며, 이 위치에 UDS SID와 DID가 있습니다.
        if len(response) > 15 and response[12:15] == b'\x62\x00\x01':
            adc_value = struct.unpack('>H', response[15:17])[0]
            print(f"\n---> SUCCESS! Received VAR: {adc_value} <---")
        else:
            print("\n---> FAILED: Did not receive a valid VAR response.")

# 통신 중 어떤 종류의 오류라도 발생하면 이곳에서 처리합니다.
except Exception as e:
    print(f"\nAn error occurred: {e}")

# 프로그램이 정상적으로 또는 오류로 종료되었음을 알립니다.
print("\nClient finished.")