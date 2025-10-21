# 모든 DoIP/UDS 메시지를 저장하는 창고 담당
# 1. Routing Activation
# 2. UDS

import struct # OffSetValue를 해석하는 함수에 의해 사용

# 1. Routing Activation

# Routing Activation 요청
REQ_ROUTING = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x00, 0x05,              # 페이로드 타입: Request Routing Activation
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 진단기의 논리 주소
    0x00,                    # Activation Type
    0x00, 0x00, 0x00, 0x00   # 예약 필드(0000)
])

# Routing Activation 응답 (Successfully activated)
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
# 현재 프로젝트에서는 진단기와 게이트웨이, ECU가 모두 일대일의 관계로 있기 때문에
# 서버 접속에 성공한 경우 Routing Activation에 실패할 수 없음 (실패하는 경우가 없음)


# 2. UDS

# DiagnosticSessionControl - Default Session
REQ_SESSION_DEFAULT = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x06,  # 페이로드 길이: 6

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x10, 0x01               # UDS Payload: SID(10), SubFunction(01)
])

# DiagnosticSessionControl - Extended Session
REQ_SESSION_EXTENDED = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x06,  # 페이로드 길이: 6

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x10, 0x03               # UDS Payload: SID(10), SubFunction(03)
])

# TesterPresent
REQ_TESTER_PRESENT = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x06,  # 페이로드 길이: 6

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x3E, 0x80               # UDS Payload: SID(3E), SubFunction(80)
])

# ReadDatabyIdentifier - 차대번호(VIN)
REQ_READ_VIN = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x22, 0xF1, 0x90         # UDS Payload: SID(22), DID(F190)
])

# ReadDatabyIdentifier - ECU의 하드웨어 버전
REQ_READ_ECU_HW = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x22, 0xF1, 0x93         # UDS Payload: SID(22), DID(F193)
])

# ReadDatabyIdentifier - ECU의 소프트웨어 버전
REQ_READ_ECU_SW = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x22, 0xF1, 0x95         # UDS Payload: SID(22), DID(F195)
])

# ReadDatabyIdentifier - ECU의 일련 번호
REQ_READ_ECU_NUM = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x22, 0xF1, 0x8C         # UDS Payload: SID(22), DID(F18C)
])

# ReadDatabyIdentifier - 시스템 공급업체 ID
REQ_READ_ID = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x22, 0xF1, 0x8A         # UDS Payload: SID(22), DID(F18A)
])

# ReadDatabyIdentifier - 측후방 초음파 센서 측정 값 (좌, 우, 후방)
REQ_READ_ULTRASOUND = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x22, 0x00, 0x01         # UDS Payload: SID(22), DID(0001)
])

# ReadDatabyIdentifier - ToF 센서 측정 값
REQ_READ_TOF = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x22, 0x00, 0x02         # UDS Payload: SID(22), DID(0002)
])

# ReadDatabyIdentifier - 엔코더 RPM 값 (좌, 우)
REQ_READ_RPM = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x22, 0x00, 0x03         # UDS Payload: SID(22), DID(0003)
])

# ReadDatabyIdentifier - 후미등 활성 여부 (좌, 우)
REQ_READ_LED = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x22, 0x00, 0x04         # UDS Payload: SID(22), DID(0004)
])

# ReadDatabyIdentifier - AEB 작동 offset 값 (가중치가 더해지기 전 기준 거리 값)
REQ_READ_OFFSET = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x22, 0x00, 0x05         # UDS Payload: SID(22), DID(0005)
])

# WriteDataByIdentifier - AEB 작동 offset 값 변경
# AEB 작동 offset 값 변경 요청 메시지를 생성하는 함수
def create_req_write_offset (offset_value):
    # 사용자로부터 받은 값을 포함하는 DoIP 요청 메시지를 생성합니다
    # offset_value: ECU에 쓰고자 하는 정수 값 (0 ~ 65535)

    uds_header = bytes([
        0x2E, 0x10, 0x01
    ])

    # struct.pack(): 숫자를 C언어 구조체처럼 바이트로 변환
    # '>H': 빅엔디안(>) 방식의 부호없는 2바이트 정수(H)로 변환
    uds_data = struct.pack('>H', offset_value)

    uds_payload = uds_header + uds_data
    doip_payload = bytes([
        0x0E, 0x00,
        0x20, 0x00
    ]) + uds_payload

    doip_header = bytes([
        0x02, 0xfd,
        0x80, 0x01
    ]) + struct.pack('>I', len(doip_payload)) # 페이로드 길이를 4바이트 정수로 변환

    doip_message = doip_header + doip_payload
    return doip_message

# ReadDTCInformation - 전체 DTC 개수 확인
REQ_READ_DTC_CNT = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x19, 0x01, 0xFF         # UDS Payload: SID(19), SubFunction(01), DTC StatusMask(FF)
])

# ReadDTCInformation - 전체 DTC 목록 확인
REQ_READ_DTC_LIST = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x07,  # 페이로드 길이: 7

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x19, 0x02, 0xFF         # UDS Payload: SID(19), SubFunction(02), DTC StatusMask(FF)
])

# ClearDiagnosticInformation
REQ_CLEAR_DTC = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x08,  # 페이로드 길이: 8

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x14, 0xFF, 0xFF, 0xFF   # UDS Payload: SID(14), GroupOfDTC(FFFFFF)
])

# ControlDTCSetting - DTC 기록 기능 활성화
REQ_SET_DTC_ACT = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x06,  # 페이로드 길이: 6

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x85, 0x01               # UDS Payload: SID(85), SubFunction(01)
])

# ControlDTCSetting - DTC 기록 기능 활성화
REQ_SET_DTC_DACT = bytes([
    0x02, 0xfd,              # 프로토콜 버전
    0x80, 0x01,              # 페이로드 타입: Diagnostic Message
    0x00, 0x00, 0x00, 0x06,  # 페이로드 길이: 6

    # --- 페이로드 ---
    0x0E, 0x00,              # 소스 논리 주소 (진단기)
    0x20, 0x00,              # 타겟 논리 주소 (ECU)
    0x85, 0x02               # UDS Payload: SID(85), SubFunction(02)
])