# 응답 메시지 분석을 담당
# 우편물 분류 센터라고 생각해볼 수 있다 (도착한 UDS를 열어보고 양식을 확인해서 내용을 분류하고 정리)
import doip_messages
import struct

def parse_routing (message):
    # routing activation 성공 여부
    if message == doip_messages.REP_ROUTING:
        return True
    else:
        return False 
    # PC와 ECU가 일대일의 관계 (하나밖에 없음)
    # PC가 게이트웨이 서버에 접속할 수만 있다면 무조건 routing activation에 성공하게 됨
    # 따라서 실패 요인을 분석하지 않음 (여러 ECU를 사용하게 된다면 수정 필요)

def parse_uds_response (message):
    
    # message가 비어있거나 최소길이보다 짧은 경우 (헤더 8 + 주소 4 + SID 1)
    if not message or len(message) < 13:
        return {
            'status' : 'error',
            'message' : 'Invalid or too short DoIP message'
        }
    
    # DoIP 페이로드 타입 분석
    payload_type = struct.unpack('>H', message[2:4])[0]
    # 2번 index에서 시작해서 4번 index 앞에서 자름
    # [0]은 첫 번째 값 꺼내기 → unpack은 값을 반환할 때 튜플(괄호로 감싸진 수정 불가능한 리스트와 유사)이라는 형태로 반환, 튜플 포장에서 0번째 값을 꺼냄
    # 숫자값으로 비교하고 싶다면 unpack / 바이트 덩어리로 비교하려면 슬라이스 (그냥 자름)
    if payload_type != 0x8001:
        return {
            'status' : 'error',
            'message' : 'Not a diagnostic message'
        }
    
    # UDS 페이로드 추출 (DoIP 헤더(8)와 페이로드의 소스/타겟 주소(4)를 제외)
    uds_payload = message[12:]
    if not uds_payload:
        return {
            'status' : 'error',
            'message' : 'Empty UDS payload after slicing'
        }

    # SID 분석
    response_sid = uds_payload[0] # 1 byte만 꺼내는 경우 숫자로 나옴 (여러 바이트는 바이트 꾸러미로 나옴)
    
    # 부정 응답 (NRC) 처리
    if response_sid == 0x7F:
        failure_sid = uds_payload[1]
        nrc = uds_payload[2]

        if nrc == 0x11:
            return {
                'status' : 'negative_response',
                'sid' : failure_sid,
                'nrc' : nrc,
                'nrc_message' : '요청하신 서비스를 지원하지 않습니다!'
            }
        
        elif nrc == 0x12:
            return {
                'status' : 'negative_response',
                'sid' : failure_sid,
                'nrc' : nrc,
                'nrc_message' : '요청하신 SID를 지원하지만 세부 Sub Function이 허용된 범위를 벗어났습니다!'
            }
        
        elif nrc == 0x31:
            return {
                'status' : 'negative_response',
                'sid' : failure_sid,
                'nrc' : nrc,
                'nrc_message' : '요청하신 SID를 지원하지만 세부 DID가 허용된 범위를 벗어났습니다!'
            }
        
        elif nrc == 0x22:
            return {
                'status' : 'negative_response',
                'sid' : failure_sid,
                'nrc' : nrc,
                'nrc_message' : '요청을 처리하기 위한 전제 조건이 만족되지 않아 명령을 수행할 수 없습니다!'
            }
        
        elif nrc == 0x7F:
            return {
                'status' : 'negative_response',
                'sid' : failure_sid,
                'nrc' : nrc,
                'nrc_message' : '현재 활성 세션에서는 해당 서비스를 지원하지 않습니다!'
            }

        else:
            return {
                'status' : 'unknown_negative_response',
                'sid' : failure_sid,
                'nrc' : nrc,
                'nrc_message' : '-'
            }
    
    # 긍정 응답 처리
    original_sid = response_sid - 0x40

    # ReadDatabyIdentifier
    if original_sid == 0x22:
        did = struct.unpack('>H', uds_payload[1:3])[0]
        data = uds_payload[3:]

        if did == 0xF190:
            vin_string = data.decode('utf-8', errors = 'ignore') # 오류는 무시
            return {
                'status' : 'success',
                'sid' : original_sid,
                'did' : did,
                'service' : 'Read Data by Identifier - 차대번호(VIN)',
                'value' : vin_string
            }

        elif did == 0xF193:
            HW_string = data.decode('utf-8', errors = 'ignore') # 오류는 무시
            return {
                'status' : 'success',
                'sid' : original_sid,
                'did' : did,
                'service' : 'Read Data by Identifier - ECU HW version',
                'value' : HW_string
            }

        elif did == 0xF195:
            SW_string = data.decode('utf-8', errors = 'ignore') # 오류는 무시
            return {
                'status' : 'success',
                'sid' : original_sid,
                'did' : did,
                'service' : 'Read Data by Identifier - ECU SW version',
                'value' : SW_string
            }

        elif did == 0xF18C:
            ECUID_string = data.decode('utf-8', errors = 'ignore') # 오류는 무시
            return {
                'status' : 'success',
                'sid' : original_sid,
                'did' : did,
                'service' : 'Read Data by Identifier - ECU 일련번호',
                'value' : ECUID_string
            }

        elif did == 0xF18A:
            ID_string = data.decode('utf-8', errors = 'ignore') # 오류는 무시
            return {
                'status' : 'success',
                'sid' : original_sid,
                'did' : did,
                'service' : 'Read Data by Identifier - 시스템 공급업체 ID',
                'value' : ID_string
            }

        elif did == 0x0001:
            left = struct.unpack('>H', data[0:2])[0]
            right = struct.unpack('>H', data[2:4])[0]
            back = struct.unpack('>H', data[4:6])[0]
            return {
                'status' : 'success',
                'sid' : original_sid,
                'did' : did,
                'service' : 'Read Data by Identifier - 초음파 센서 측정 값',
                'value' : f'Left: {left} cm / Right: {right} cm / Back: {back} cm'
            }

        elif did == 0x0002:
            tof = struct.unpack('>H', data[0:2])[0]
            return {
                'status' : 'success',
                'sid' : original_sid,
                'did' : did,
                'service' : 'Read Data by Identifier - TOF 센서 측정 값',
                'value' : f'Front: {tof} mm'
            }

        elif did == 0x0003:
            left_rpm = struct.unpack('>H', data[0:2])[0]
            right_rpm = struct.unpack('>H', data[2:4])[0]
            return {
                'status' : 'success',
                'sid' : original_sid,
                'did' : did,
                'service' : 'Read Data by Identifier - 엔코더 RPM 측정 값',
                'value' : f'Left RPM: {left_rpm} rpm / Right RPM: {right_rpm} rpm'
            }

        elif did == 0x0004:
            left_led = data[0]
            right_led = data[1]
            return {
                'status' : 'success',
                'sid' : original_sid,
                'did' : did,
                'service' : 'Read Data by Identifier - 후미등 활성 여부',
                'value' : f'1(On), 0(Off) / Left LED: {left_led} / Right LED: {right_led}'
            }

        elif did == 0x0005:
            value = struct.unpack('>H', data[0:2])[0]
            return {
                'status' : 'success',
                'sid' : original_sid,
                'did' : did,
                'service' : 'Read Data by Identifier - AEB 작동 offset 값',
                'value' : f'AEB offset: {value} mm'
            }

        else:
            return {
                'status' : 'unknown_positive_response_0x22',
                'sid' : original_sid,
                'did' : did,
                'service' : 'unkown',
                'value' : '-'
            }

    # ReadDatabyIdentifier
    elif original_sid == 0x2E:
        did = struct.unpack('>H', uds_payload[1:3])[0]
        
        if did == 0x1001:
            return {
                'status' : 'success',
                'sid' : original_sid,
                'did' : did,
                'service' : 'Write Data by Identifier - AEB 작동 offset 변경',
                'value' : 'AEB offset을 성공적으로 변경했습니다!'
            }
        else:
            return {
                'status' : 'unknown_positive_response_0x2E',
                'sid' : original_sid,
                'did' : did,
                'service' : 'unknown',
                'value' : '-'
            }
       
    # ReadDTCInformation
    elif original_sid == 0x19:
        subFunction = uds_payload[1]
        dtcStatusMask = uds_payload[2]
        dtcFormat = uds_payload[3]

        if subFunction == 0x01:
            value = struct.unpack('>H', uds_payload[4:6])[0]
            return {
                'status' : 'success',
                'sid' : original_sid,
                'sub_function' : subFunction,
                'status_mask' : dtcStatusMask,
                'dtc_format' : dtcFormat,
                'service' : 'Read DTC Information - 전체 DTC 개수 확인',
                'value' : f'DTC: {value} 건'
            }

        elif subFunction == 0x02:
            # DTC: sid + subfunction + mask + 4byte씩 데이터(4n bit)
            # --- 4byte의 데이터 중 --- 
            # 3번째 byte는 0x13 고정 (회로가 끊어짐 → 부품 연결이 끊어짐)
            # 4번째 byte는 0x08 고정 (확정된 고장)
            # 1번째 byte + 2번째 byte: 상위 2bit (고장 코드가 속한 시스템) + 하위 14bit (고장 번호/DTC 번호)

            dtc_list = []
            dtc_data_bytes = uds_payload[3:] # 실제 DTC 데이터만 가져옴

            for i in range(0, len(dtc_data_bytes), 4):
                chunk = dtc_data_bytes[i:i+4]
                if len(chunk) < 4:
                    continue # 데이터가 4바이트 미만이라면 건너뜀

                # 원래는 받아온 데이터를 파싱해야 하지만 (해석 필요)
                # DTC 메시지로 6개만 사용하기 때문에 if 문을 이용
                dtc_message = struct.unpack('>I', chunk[0:])[0]

                if dtc_message == 0x9A111308:
                    dtc_list.append({
                        'dtc_code' : 'B1A11',
                        'failure_type' : '0x13',
                        'status_mask' : '0x08',
                        'dtc_explain' : '좌측 초음파 센서가 연결되어 있지 않습니다.'
                    })
                elif dtc_message == 0x9A121308:
                    dtc_list.append({
                        'dtc_code' : 'B1A12',
                        'failure_type' : '0x13',
                        'status_mask' : '0x08',
                        'dtc_explain' : '우측 초음파 센서가 연결되어 있지 않습니다.'
                    })
                elif dtc_message == 0x9A131308:
                    dtc_list.append({
                        'dtc_code' : 'B1A13',
                        'failure_type' : '0x13',
                        'status_mask' : '0x08',
                        'dtc_explain' : '후방 초음파 센서가 연결되어 있지 않습니다.'
                    })
                elif dtc_message == 0x5B311308:
                    dtc_list.append({
                        'dtc_code' : 'C1B31',
                        'failure_type' : '0x13',
                        'status_mask' : '0x08',
                        'dtc_explain' : '전방 ToF 센서가 연결되어 있지 않습니다.'
                    })
                elif dtc_message == 0x40351308:
                    dtc_list.append({
                        'dtc_code' : 'C0035',
                        'failure_type' : '0x13',
                        'status_mask' : '0x08',
                        'dtc_explain' : '좌측 전방 휠 속도 센서가 연결되어 있지 않습니다.'
                    })
                elif dtc_message == 0x40401308:
                    dtc_list.append({
                        'dtc_code' : 'C0035',
                        'failure_type' : '0x13',
                        'status_mask' : '0x08',
                        'dtc_explain' : '우측 전방 휠 속도 센서가 연결되어 있지 않습니다.'
                    })
                else:
                    dtc_list.append({
                        'dtc_code' : '-',
                        'failure_type' : '-',
                        'status_mask' : '-',
                        'dtc_explain' : '알 수 없는 DTC 코드가 기록되었습니다.'
                    })

            return {
                'status' : 'success',
                'sid' : original_sid,
                'sub_function' : subFunction,
                'status_mask' : dtcStatusMask,
                'dtc_format' : dtcFormat,
                'service' : 'Read DTC Information - 전체 DTC 목록 확인',
                'value' : dtc_list
            }

        else:
            return {
                'status' : 'unknown_positive_response_0x19',
                'sid' : original_sid,
                'sub_function' : subFunction,
                'service' : 'unknown',
                'value' : '-'
            }

    # ClearDiagnosticInformation
    elif original_sid == 0x14:
        # ClearDiagnosticInformation의 경우 응답 UDS payload에 SID만 붙어서 옴
        return {
                'status' : 'success',
                'sid' : original_sid,
                'service' : 'ClearDiagnosticInformation - 전체 DTC 기록 삭제',
                'value' : '전체 DTC 기록을 성공적으로 삭제했습니다!'
            }

    # ControlDTCSetting
    elif original_sid == 0x85:
        subFunction = uds_payload[1]

        if subFunction == 0x01:
            return {
                'status' : 'success',
                'sid' : original_sid,
                'sub_function' : subFunction,
                'service' : 'ControlDTCSetting - DTC 기록 활성화',
                'value' : 'DTC 기록을 성공적으로 활성화했습니다!'
            }
        
        elif subFunction == 0x02:
            return {
                'status' : 'success',
                'sid' : original_sid,
                'sub_function' : subFunction,
                'service' : 'ControlDTCSetting - DTC 기록 비활성화',
                'value' : 'DTC 기록을 성공적으로 비활성화했습니다!'
            }

        else:
            return {
                'status' : 'unknown_positive_response_0x85',
                'sid' : original_sid,
                'sub_function' : subFunction,
                'service' : 'unknown',
                'value' : '-'
            }
    
    # DiagnosticSessionControl
    elif original_sid == 0x10:
        subFunction = uds_payload[1]

        if subFunction == 0x01:
            return {
                'status' : 'success',
                'sid' : original_sid,
                'sub_function' : subFunction,
                'service' : 'DiagnosticSessionControl - Default Session으로 변경',
                'value' : 'Session을 성공적으로 변경했습니다!'
            }
        
        elif subFunction == 0x03:
            return {
                'status' : 'success',
                'sid' : original_sid,
                'sub_function' : subFunction,
                'service' : 'DiagnosticSessionControl - Extended Session으로 변경',
                'value' : 'Session을 성공적으로 변경했습니다!'
            }
        
        else:
            return {
                'status' : 'unknown_positive_response_0x10',
                'sid' : original_sid,
                'sub_function' : subFunction,
                'service' : 'unknown',
                'value' : '-'
            }

    # else unknown
    else:
        return {
            'status' : 'unknown_positive_response',
            'sid' : original_sid,
            'service' : 'unknown',
            'value' : '-'
        }