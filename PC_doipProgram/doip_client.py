# 실제 통신을 담당
import socket
import time
import threading # 멀티스레딩
import doip_messages
import uds_parser

# ----------멀티스레딩 개념을 적용하여 구성----------
# 1. 사용자의 입력을 기다리는 작업 (메인 스레드)
# 2. Extended Session에 진입한 경우 주기적으로 TesterPresent 전송 (TesterPresent 스레드)
# 두 가지 일을 동시에 해야하므로 멀티스레딩 개념을 적용함
# 이때 두 스레드가 socket을 공유 자원으로 사용 → Lock 사용 (동기화)

# 통신 설정
SERVER_HOST = '192.168.137.77' # 게이트웨이(라즈베리 파이 4)의 IP 주소
SERVER_PORT = 13400

# 공유 자원 (socket)을 관리하기 위한 Lock
socket_lock = threading.Lock()

# ----------백그라운드(TesterPresent) 스레드 함수----------
def tester_present_worker(sk, stop_event):
    # 백그라운드에서 2초마다 TesterPresent를 보내는 함수
    # sk: 통신에 사용될 소켓 객체
    # stop_event: 스레드를 중지시키기 위한 신호(Event) 객체

    while not stop_event.is_set():
        # 2초 동안 대기
        # 만약 중간에 stop_event.set()이 호출되면 대기 종료 (stop_event.wail을 사용하기 때문)
        stop_event.wait(2.0)

        if stop_event.is_set():
            break

        try:
            with socket_lock:
                print("\n.....Background...Tester Present......")
                sk.sendall(doip_messages.REQ_TESTER_PRESENT)

        except Exception as e:
            print(f".....Error...Background...{e}.....")
            break

    print(".....Close...Background")

# ----------메인(main) 스레드 함수----------
def main():
    tester_thread = None # TesterPresent 스레드를 담을 변수 
    stop_tester_present = threading.Event() # TesterPresent 스레드를 제어하기 위한 신호 객체
    # Event 객체는 스레드 간의 간단한 신호를 주고받는 용도
    # .set(): 멈추라는 신호 (빨간 신호등)
    # .clear(): 계속 진행하라는 신호 (초록 신호등)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:

        # ----------서버 접속 및 Routing Activation----------
        try:
            # 게이트웨어 서버 접속
            print(f"게이트웨이({SERVER_HOST}:{SERVER_PORT})에 연결을 시도합니다...")
            sock.connect((SERVER_HOST, SERVER_PORT))
            print("게이트웨이에 성공적으로 연결되었습니다!")

            # Routing Activation
            sock.settimeout(5.0) # 소켓에 5초 타임아웃 설정 (5초 이상 응답이 없으면 예외처리 (블로킹 방지))

            print("Routing Activation을 시도합니다...")
            sock.sendall(doip_messages.REQ_ROUTING)
            
            try:
                response = sock.recv(1024) # 최대 1024 bytes까지 데이터를 읽어옴
            except socket.timeout:
                print('Error: 서버로부터 응답 시간 초과 (Timeout)')
                response = None

            if response:
                if uds_parser.parse_routing(response):
                    print("Success: Routing Activation")
                else:
                    print("Failure: Routing Activation에 실패했습니다! (잘못된 응답 발생)")
            else:
                return
            
        except ConnectionRefusedError:
            print('Failure: 연결 실패! \n서버가 실행 중인지, IP 주소가 맞는지 확인해주세요!')
            return
        
        except Exception as e:
            print(f"Failure: 연결 중 오류 발생: {e}")
            return

        # ----------메인 루프----------
        while True:
            # 진단 메뉴판 (리팩토링할 때 GUI로 넘어가야 함)
            print("\n======================================")
            print(" 원하는 진단 명령을 선택하세요!")
            print(" s1: Default Session으로 변경")
            print(" s3: Extended Session으로 변경")
            print(" ------------------------------------")
            print(" 1: 차대번호(VIN) 읽기")
            print(" 2: ECU 하드웨어 버전 읽기")
            print(" 3: ECU의 소프트웨어 버전 읽기")
            print(" 4: ECU의 일련 번호 읽기")
            print(" 5: 시스템 공급업체 ID 읽기")
            print(" 6: 측후방 초음파 센서 측정 값 (좌, 우, 후방) 읽기")
            print(" 7: ToF 센서 측정 값 읽기")
            print(" 8: 엔코더 RPM 값 (좌, 우) 읽기")
            print(" 9: 후미등 활성 여부 (좌, 우) 읽기")
            print(" 10: AEB 작동 offset 값 읽기")
            print(" 11: AEB 작동 offset 값 변경")
            print(" 12: 전체 DTC 개수 확인")
            print(" 13: 전체 DTC 목록 확인")
            print(" 14: 전체 DTC 삭제")
            print(" 15: DTC 기록 기능 활성화")
            print(" 16: DTC 기록 기능 비활성화")

            print(" 0: 프로그램 종료")
            print("======================================\n")
            # -----------------------------------------------

            try:
                choice = input("진단 명령 선택: ") # 반환 값은 무조건 문자열(string / 참고로 파이썬에서는 ''도 문자열로 인식 가능)
                message_to_send = None

                # ---프로그램 종료---
                if choice == '0':
                    print("프로그램을 종료합니다!")
                    break

                # ---세션 관리---
                if choice == 's3':
                    # tester_thread가 비활성화된 경우
                    if tester_thread is None or not tester_thread.is_alive():
                        print("\nExtended Session으로 변경을 요청합니다...")
                        try:
                            result0 = None
                            with socket_lock:
                                sock.sendall(doip_messages.REQ_SESSION_EXTENDED)
                                response0 = sock.recv(1024)
                            result0 = uds_parser.parse_uds_response(response0)

                        except socket.timeout:
                            print("Error: 세션을 변경하지 못했습니다! (Timeout)")
                        except Exception as e:
                            print(f"Error: 세션을 변경하지 못했습니다! ({e})")

                        print("----- 응답 결과 -----")
                        print(result0)
                        print("--------------------")

                        if result0.get('status') == 'success':
                            print("Extended Session 진입. TesterPresent를 시작합니다.")

                            stop_tester_present.clear() # 중지 신호 초기화
                            # 새로운 스레드 객체 생성
                            # target = 스레드가 수행해야 할 내용 (함수)
                            # args = 함수의 입력값들
                            tester_thread = threading.Thread(target=tester_present_worker, args=(sock, stop_tester_present))
                            tester_thread.daemon = True 
                            # 생성한 스레드를 Daemon Thread로 설정 (보조/백그라운드 스레드로 설정)
                            # False로 설정할 경우 (메인 프로그램(메인 스레드)이 끝나도 데몬이 아닌 스레드는 자기 일이 끝날 때까지 수행함)
                            tester_thread.start() # tester_thread 실행
                        else:
                            print("Extended Session으로 진입하지 못했습니다")
                    else:
                        print("\n이미 Extended Session 입니다!")
                    continue

                if choice == 's1':
                    # tester_thread가 비활성화된 경우
                    if tester_thread is not None and tester_thread.is_alive():
                        print("\nTesterPresent를 종료합니다...")
                        # tester_thread 종료 신호를 보냄
                        stop_tester_present.set()
                        # join 메소드: 메인 스레드가 보조 스레드의 일이 끝날 때까지 기다리도록 만드는 기능
                        # timeout=1.0: 최대 1초 동안 기다림
                        tester_thread.join(timeout=1.0)
                        tester_thread = None

                        try:
                            result1 = None
                            with socket_lock:
                                sock.sendall(doip_messages.REQ_SESSION_DEFAULT)
                                response1 = sock.recv(1024)
                            result1 = uds_parser.parse_uds_response(response1)

                        except socket.timeout:
                            print("Error: 세션을 변경하지 못했습니다! (Timeout)")
                        except Exception as e:
                            print(f"Error: 세션을 변경하지 못했습니다! ({e})")

                        print("----- 응답 결과 -----")
                        print(result1)
                        print("--------------------")

                        if result1.get('status') == 'success':
                            print("Default Session으로 복귀했습니다.")
                        else:
                            print("Defualt Session으로 복귀하지 못했습니다.")
                    else:
                        print("\n이미 Default Session 입니다!")
                    continue
                        
                # ---일반 진단 명령---
                if choice == '1':
                    message_to_send = doip_messages.REQ_READ_VIN
                elif choice == '2':
                    message_to_send = doip_messages.REQ_READ_ECU_HW
                elif choice == '3':
                    message_to_send = doip_messages.REQ_READ_ECU_SW 
                elif choice == '4':
                    message_to_send = doip_messages.REQ_READ_ECU_NUM      
                elif choice == '5':
                    message_to_send = doip_messages.REQ_READ_ID
                elif choice == '6':
                    message_to_send = doip_messages.REQ_READ_ULTRASOUND
                elif choice == '7':
                    message_to_send = doip_messages.REQ_READ_TOF
                elif choice == '8':
                    message_to_send = doip_messages.REQ_READ_RPM
                elif choice == '9':
                    message_to_send = doip_messages.REQ_READ_LED
                elif choice == '10':
                    message_to_send = doip_messages.REQ_READ_OFFSET
                elif choice == '11':
                    # AEB 작동 offset 값 변경
                    value = input("새로운 offset 값(숫자만 입력, 1000cm 이하 값): ")
                    message_to_send = doip_messages.create_req_write_offset(int(value))
                elif choice == '12':
                    message_to_send = doip_messages.REQ_READ_DTC_CNT
                elif choice == '13':
                    message_to_send = doip_messages.REQ_READ_DTC_LIST
                elif choice == '14':
                    message_to_send = doip_messages.REQ_CLEAR_DTC
                elif choice == '15':
                    message_to_send = doip_messages.REQ_SET_DTC_ACT
                elif choice == '16':
                    message_to_send = doip_messages.REQ_SET_DTC_DACT

                else:
                    print("Error: 잘못된 입력입니다. 다시 선택해주세요.")
                    continue

                # ---요청 전송 및 응답 결과---
                print("\n요청을 전송합니다...")
                # 데이터를 보내기 전 Lock 획득 (모든 과정 종료 후 자동으로 Lock 반환)
                # 응답을 받는 동안 TesterPresent 전송도 방지 (RaceCondition 발생 가능)
                try:
                    result = None

                    with socket_lock:
                        sock.sendall(message_to_send)
                        response = sock.recv(1024)
                    result = uds_parser.parse_uds_response(response)

                except socket.timeout:
                    result = {
                        'status' : 'error',
                        'message' : 'Timeout (Response Timeout)'
                    }
                except Exception as e:
                    result = {
                        'status' : 'error',
                        'message' : f'error message: {e}'
                    }

                print("----- 응답 결과 -----")
                print(result)
                print("--------------------")

            except ConnectionResetError:
                print("Error: 서버와의 연결이 끊어졌습니다. 프로그램을 종료합니다.")
                break

            except Exception as e:
                print(f"Error: 통신 중 오류 발생: {e}")
                break

        # ----------프로그램 종료 전 마무리 작업----------
        if tester_thread is not None and tester_thread.is_alive():
            stop_tester_present.set()
            tester_thread.join(timeout=1.0)

# ----------프로그램 시작 (main)----------
if __name__ == "__main__":
    main()