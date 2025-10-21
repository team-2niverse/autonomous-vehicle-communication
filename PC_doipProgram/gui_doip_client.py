# doip_client.py 리팩토링
# 진단기(PC) 클라이언트 코드를 GUI와 연동할 수 있도록 재구성
# 진단기의 통신 흐름은 doip_client.py를 참고

import socket
import threading
import time
import doip_messages
import uds_parser

class DoIPClient:
    def __init__(self, host, port, log_callback):
        self.host = host
        self.port = port
        self.log_callback = log_callback
        self.sock = None
        self.is_connected = False
        self.tester_thread = None
        self.stop_tester_present = threading.Event()
        self.socket_lock = threading.Lock()
        self.tester_present_count = 0

    def _tester_present_worker(self):
        self.log_callback("(백그라운드) TesterPresent 스레드 시작.", level='STATUS')
        while not self.stop_tester_present.is_set():
            self.stop_tester_present.wait(2.0)
            if self.stop_tester_present.is_set():
                break
            try:
                with self.socket_lock:
                    if self.sock and self.is_connected:
                        self.sock.sendall(doip_messages.REQ_TESTER_PRESENT)
                        self.tester_present_count += 1
                        self.log_callback(f"TesterPresent sent (count: {self.tester_present_count})", level='STATUS')
            except Exception as e:
                self.log_callback(f"(백그라운드) 오류: {e}", level='ERROR')
                self.stop_tester_present.set()
                break
        self.log_callback("(백그라운드) TesterPresent 전송 스레드 종료.", level='INFO')

    def connect(self):
        if self.is_connected: return False, "이미 연결되어 있습니다."
        try:
            self.log_callback(f"게이트웨이({self.host}:{self.port})에 연결을 시도합니다...")
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.settimeout(5.0)
            self.sock.connect((self.host, self.port))
            self.log_callback("Routing Activation을 요청합니다...")
            self.sock.sendall(doip_messages.REQ_ROUTING)
            response = self.sock.recv(1024)
            if uds_parser.parse_routing(response):
                self.is_connected = True
                self.log_callback("✅ 라우팅이 성공적으로 활성화되었습니다.")
                return True, "연결 성공"
            else:
                self.sock.close()
                self.log_callback("❌ 라우팅 활성화에 실패했습니다.")
                return False, "라우팅 활성화 실패"
        except Exception as e:
            self.log_callback(f"❌ 연결 중 오류 발생: {e}")
            return False, str(e)

    def disconnect(self):
        if not self.is_connected: return
        self.stop_tester_present_thread()
        if self.sock:
            try:
                self.sock.shutdown(socket.SHUT_RDWR)
            except OSError:
                pass
            self.sock.close()
        self.is_connected = False
        self.sock = None
        self.log_callback("연결이 종료되었습니다.")

    def send_diagnostic_request(self, message):
        if not self.is_connected: return {'status': 'error', 'message': '연결되어 있지 않습니다.'}
        try:
            with self.socket_lock:
                self.sock.sendall(message)
                response = self.sock.recv(1024)
            return uds_parser.parse_uds_response(response)
        except socket.timeout:
            return {'status': 'error', 'message': '응답 시간 초과 (Timeout)'}
        except Exception as e:
            self.log_callback(f"통신 오류 발생: {e}. 연결을 종료합니다.")
            self.disconnect()
            return {'status': 'error', 'message': f'통신 오류 발생: {e}'}

    def start_tester_present_thread(self):
        if not (self.tester_thread and self.tester_thread.is_alive()):
            self.tester_present_count = 0
            self.stop_tester_present.clear()
            self.tester_thread = threading.Thread(target=self._tester_present_worker, daemon=True)
            self.tester_thread.start()

    def stop_tester_present_thread(self):
        if self.tester_thread and self.tester_thread.is_alive():
            self.stop_tester_present.set()
            self.tester_thread.join(timeout=1.0)
            self.tester_thread = None