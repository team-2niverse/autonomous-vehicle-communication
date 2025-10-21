# -*- coding: utf-8 -*-


import struct
import socket
import threading
import time
from enum import Enum

# SOME/IP def
class MessageType(Enum):
    REQUEST = 0x00
    REQUEST_NO_RETURN = 0x01
    NOTIFICATION = 0x02
    RESPONSE = 0x80
    ERROR = 0x81
    
class ReturnCode(Enum):
    E_OK = 0x00
    E_NOT_OK = 0x01
    E_UNKNOWN_SERVICE = 0x02
    E_UNKNOWN_METHOD = 0x03
    E_NOT_READY = 0x04
    E_NOT_REACHABLE = 0x05
    E_TIMEOUT = 0x06

class SomeIPMessage:
    """SOME/IP message structure"""
    HEADER_SIZE = 16
    PROTOCOL_VERSION = 0x01
    
    def __init__(self, 
                 service_id=0, 
                 method_id=0,
                 client_id=0,
                 session_id=0,
                 interface_version=1,
                 message_type=MessageType.REQUEST,
                 return_code=ReturnCode.E_OK,
                 payload=b''):
        self.service_id = service_id
        self.method_id = method_id
        self.client_id = client_id
        self.session_id = session_id
        self.protocol_version = self.PROTOCOL_VERSION
        self.interface_version = interface_version
        self.message_type = message_type
        self.return_code = return_code
        self.payload = payload
    
    def to_bytes(self):
        """message to byte stream serialization"""
        message_id = (self.service_id << 16) | self.method_id
        length = len(self.payload) + 8  # 페이로드 + 나머지 헤더
        request_id = (self.client_id << 16) | self.session_id
        
        header = struct.pack(
            '!IIIBBBB',
            message_id,
            length,
            request_id,
            self.protocol_version,
            self.interface_version,
            self.message_type.value if isinstance(self.message_type, MessageType) else self.message_type,
            self.return_code.value if isinstance(self.return_code, ReturnCode) else self.return_code
        )
        return header + self.payload
    
    @classmethod
    def from_bytes(cls, data):
        """back - serialization """
        if len(data) < cls.HEADER_SIZE:
            raise ValueError(f"Invalid SOME/IP message: too short ({len(data)} bytes)")
        
        header = struct.unpack('!IIIBBBB', data[:cls.HEADER_SIZE])
        
        message_id = header[0]
        length = header[1]
        request_id = header[2]
        
        msg = cls()
        msg.service_id = (message_id >> 16) & 0xFFFF
        msg.method_id = message_id & 0xFFFF
        msg.client_id = (request_id >> 16) & 0xFFFF
        msg.session_id = request_id & 0xFFFF
        msg.protocol_version = header[3]
        msg.interface_version = header[4]
        msg.message_type = MessageType(header[5])
        msg.return_code = ReturnCode(header[6])
        
        payload_length = length - 8
        msg.payload = data[cls.HEADER_SIZE:cls.HEADER_SIZE + payload_length]
        
        return msg



class ServiceDiscovery:
    """SOME/IP-SD coding """
    SD_PORT = 30490
    MULTICAST_ADDR = '224.244.224.245'
    
    def __init__(self):
        self.services = {}  # 등록된 서비스
        self.subscriptions = {}  # 구독 정보
        self.running = False
        
    def create_find_service_message(self, service_id):
        """FindService"""
        # SD 헤더 (플래그, 예약, 엔트리 수)
        sd_header = struct.pack('!BBH', 0xC0, 0x00, 0x0000)
        
        # 엔트리 배열 길이
        entries_length = struct.pack('!I', 16)  # 1개 엔트리
        
        # FindService 엔트리 (Type 0x00)
        entry = struct.pack(
            '!BBBBHHII',
            0x00,  # Type: Find Service
            0x00,  # Index 1st options
            0x00,  # Index 2nd options 
            0x00,  # # of options
            service_id,  # Service ID
            0xFFFF,  # Instance ID (any)
            0x0000000a,  # Major Version (0x01) | TTL
            0x00000001,  # Minor Version
        )
        
        payload = sd_header + entries_length + entry
        
        return SomeIPMessage(
            service_id=0xFFFF,
            method_id=0x8100,  # SD method ID
            message_type=MessageType.NOTIFICATION,
            payload=payload
        )
    
    def create_offer_service_message(self, service_id, instance_id, ip, port):
        """OfferService message creaete"""
        # SD 헤더
        sd_header = struct.pack('!BBH', 0x40, 0x00, 0x0000)
        
        # 엔트리 배열
        entries_length = struct.pack('!I', 16)
        
        # OfferService 엔트리 (Type 0x01)
        entry = struct.pack(
            '!BBBBHHHHBBBB',
            0x01,  # Type: Offer Service
            0x00,  # Index 1st options
            0x00,  # Index 2nd options
            0x01,  # # of options (1 = IPv4 endpoint)
            service_id,
            instance_id,
            0x01,  # Major Version
            0xFFFFFF,  # TTL (무한)
            0x00,  # Minor Version
        )
        
        # 옵션 배열 (IPv4 엔드포인트)
        options_length = struct.pack('!I', 12)
        
        # IPv4 옵션
        ip_bytes = socket.inet_aton(ip)
        option = struct.pack(
            '!HBB4sBBH',
            0x0009,  # Length
            0x04,    # Type: IPv4 Endpoint
            0x00,    # Reserved
            ip_bytes,
            0x00,    # Reserved
            0x06,    # L4 Protocol (TCP)
            port
        )
        
        payload = sd_header + entries_length + entry + options_length + option
        
        return SomeIPMessage(
            service_id=0xFFFF,
            method_id=0x8100,
            message_type=MessageType.NOTIFICATION,
            payload=payload
        )

    def create_subscribe_message(self, service_id, group_id, ip, port):
        """Subscribe message creaete"""
        # SD 헤더
        sd_header = struct.pack('!BBH', 0x40, 0x00, 0x0000)
        
        # 엔트리 배열
        entries_length = struct.pack('!I', 16)
        
        # OfferService 엔트리 (Type 0x01)
        entry = struct.pack(
            '!BBBBHHIHH',
            0x06,  # Type: Offer Service
            0x00,  # Index 1st options
            0x00,  # Index 2nd options
            0x10,  # # of options
            service_id,
            0x0001,
            0x00FFFFFF,  # TTL (무한)
            0x0000,  # Reserved
            group_id
        )
        
        # 옵션 배열 (IPv4 엔드포인트)
        options_length = struct.pack('!I', 12)
        
        # IPv4 옵션
        ip_bytes = socket.inet_aton(ip)
        option = struct.pack(
            '!HBB4sBBH',
            0x0009,  # Length
            0x24,    # Type: SD
            0x00,    # Reserved
            ip_bytes,
            0x00,    # Reserved
            0x11,    # L4 Protocol (UDP)
            port
        )
        
        payload = sd_header + entries_length + entry + options_length + option
        
        return SomeIPMessage(
            service_id=0xFFFF,
            method_id=0x8100,
            message_type=MessageType.NOTIFICATION,
            payload=payload
        )
    
    def _advertise_service(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
        
        while self.running:
            msg = self.sd.create_offer_service_message(
                self.service_id,
                self.instance_id,
                '127.0.0.1',
                self.port
            )
            sock.sendto(msg.to_bytes(), (self.sd.MULTICAST_ADDR, self.sd.SD_PORT))
            time.sleep(3)  # 3초마다 광고
    
    def notify_event(self, event_id, payload):
        if event_id in self.events:
            notification = SomeIPMessage(
                service_id=self.service_id,
                method_id=event_id | 0x8000,  # Event ID = Method ID | 0x8000
                message_type=MessageType.NOTIFICATION,
                payload=payload
            )
            
            # 모든 구독자에게 전송
            for subscriber in self.events[event_id]:
                try:
                    subscriber.send(notification.to_bytes())
                except:
                    self.events[event_id].remove(subscriber)





class Service:
    def __init__(self, service_id):
        self.service_id = service_id
        self.ip = '0.0.0.0'
        self.port = 0x0000
    def assignIPPort(self, ip, port):
        self.ip = ip
        self.port = port    
class Method:
    def __init__(self, service, method_id, event_group):
        self.service = service
        self.method_id = method_id
        self.instance_id = 0x0001
        self.eventgroup_id = event_group

    

Services = {0x100: Service(0x0100), 0x200:Service(0x0200)}
Methods = {0x0100: Method(Services[0x0100], 0x0100, 0x0000), 0x0101:Method(Services[0x0100], 0x0101, 0x0000), 
            0x8200:Method(Services[0x0200], 0x8200, 0x8200), 0x8201:Method(Services[0x0200], 0x8201, 0x8201),
            0x8202:Method(Services[0x0200], 0x8202, 0x8202), 0x8203:Method(Services[0x0200], 0x8203, 0x8203)}



class SomeIPClient:

    def __init__(self, client_id=0x0000):
        self.client_id = client_id
        self.session_id = 0
        self.socket = None
        self.sd = ServiceDiscovery()
        self.find_service(0xFFFF)

        self.running = False
        self.event_thread = None

    def find_service(self, service_id):
        #sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
        sock.bind(('192.168.2.10', 30490))  # 수신 포트를 고정!!!!

        #sock.settimeout(2.0)

        #local_ip = '192.168.2.15'
        #sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_IF, socket.inet_aton(local_ip))
        
        msg = self.sd.create_find_service_message(service_id)
        sock.sendto(msg.to_bytes(), ('224.224.224.245', self.sd.SD_PORT))
        #sock.sendto(msg.to_bytes(), ('192.168.2.20', self.sd.SD_PORT))

        sock.settimeout(0.5)  # 짧은 타임아웃으로 여러번 recv 시도

        start_time = time.time()
        responses = []

        while time.time() - start_time < 2.0:
            try:
                data, addr = sock.recvfrom(4096)
                response = SomeIPMessage.from_bytes(data)
                #ip = struct.unpack('!F', response.payload[61:65])
                service_id = struct.unpack('!H', response.payload[12:14])[0]
                ip  = socket.inet_ntoa(response.payload[32:36])
                port = struct.unpack('!H', response.payload[38:])[0]
                Services[service_id].assignIPPort(ip, port)
                responses.append((ip, port))
            except socket.timeout:
                # 타임아웃 시 recv 대기 중단하고 루프 종료
                break

        sock.close()
        return responses

    def sub_service(self, service_id, group_id):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind(('192.168.2.10', 30490))
        sock.settimeout(2.0)
        
        msg = self.sd.create_subscribe_message(service_id, group_id, '192.168.2.10', 30502)
        sock.sendto(msg.to_bytes(), ('192.168.2.20', 30490))
        
        try:
            data, addr = sock.recvfrom(4096)
            print(f"Subscribe success at {group_id:04x} event")
            return addr[0], 30502  # 기본 포트
        except socket.timeout:
            print("ack not found")
            return None, None

    def request_service(self, service_obj, method_id, payload=b''):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind(('192.168.2.10', service_obj.port))
        sock.settimeout(2.0)
        
        request = SomeIPMessage(
            service_id=service_obj.service_id,
            method_id=method_id,
            client_id=self.client_id,
            session_id=self.session_id,
            message_type=MessageType.REQUEST,
            payload=payload
        )
        sock.sendto(request.to_bytes(), (service_obj.ip, service_obj.port))
        
        try:
            data, addr = sock.recvfrom(4096)
            response = SomeIPMessage.from_bytes(data)
            print(f"success request at {method_id:04x}")
            return response.payload
        except socket.timeout:
            print("ack not found")
            return None

    def connect(self, host, port):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((host, port))
        print(f"Connected to {host}:{port}")
    
    def call_method(self, service_obj, method_id, payload=b''):
        self.session_id += 1
        
        request = SomeIPMessage(
            service_id=service_id,
            method_id=method_id,
            client_id=self.client_id,
            session_id=self.session_id,
            message_type=MessageType.REQUEST,
            payload=payload
        )
        
        self.socket.send(request.to_bytes())
        
        #receive response
        data = self.socket.recv(4096)
        response = SomeIPMessage.from_bytes(data)
        
        if response.return_code == ReturnCode.E_OK:
            return response.payload
        else:
            raise Exception(f"Method call failed: {response.return_code}")

    def get_event(self):
        # 1. UDP 소켓 생성 (SOCK_DGRAM)
        event_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
        # TimeOut 설정 (선택 사항: 데이터가 오랫동안 없어도 프로그램이 완전히 멈추지 않게 함)
        event_socket.settimeout(1.0) # 1초마다 타임아웃 발생 (Ctrl+C를 처리하기 위함)
    
        try:
            # 2. 소켓 바인딩 (수신 대기 시작)
            event_socket.bind(('0.0.0.0', 30502))
            print(f"UDP Event Receiver initialized. Listening continuously on {'192.168.2.10'}:{'30502'}")

            while True:
                try:
                    # 3. 데이터 수신
                    data, addr = event_socket.recvfrom(1024)
                    response = SomeIPMessage.from_bytes(data)
                    if len(data) < 4:
                        print(f"Error: Received truncated data ({len(data)} bytes). Ignoring.")
                        continue
                
                    # Method ID 추출 (Event ID)
                    # SOME/IP Header: [Service ID (2B)] [Method ID (2B)] ...
                    method_id = response.method_id
                
                    print(f"\n--- Event Received ---")
                    print(f"From: {addr[0]}:{addr[1]}")
                    if (response.method_id == 0x8200):
                        unpacked_data = struct.unpack('!ff', response.payload)
                        print(f"left rpm : {unpacked_data[0]} , right rpm : {unpacked_data[1]}")
                    elif (response.method_id == 0x8201):
                        unpacked_value = struct.unpack('!B', response.payload[:1])[0]
                        print(f"AEB Status : {unpacked_value}")
                    elif (response.method_id == 0x8202):
                        unpacked_value = struct.unpack('!B', response.payload[:1])[0]
                        print(f"Parking Status : {unpacked_value}")
                    elif (response.method_id == 0x8203):
                        unpacked_data = struct.unpack('!ff', response.payload)
                        print(f"Front Distance : {unpacked_data[0]}")
                    # TODO: 여기에 Event 페이로드 (data[16:] 이후)를 파싱하여 사용하는 로직을 추가
                
                    # 원하는 이벤트든 아니든, 처리가 끝났으므로 다음 이벤트를 대기
                    # 이 시점에서 루프는 continue되어 다시 event_socket.recvfrom(1024)로 돌아갑니다.

                except socket.timeout:
                    # 타임아웃이 발생하면, Ctrl+C와 같은 인터럽트 신호를 확인하기 위해 루프를 다시 시작합니다.
                    continue
                except KeyboardInterrupt:
                    # Ctrl+C 수신 시 루프 종료
                    break
                except Exception as e:
                    print(f"Error during reception: {e}")
                    break
                
        except Exception as e:
            print(f"Error binding socket: {e}")
        finally:
            event_socket.close()
            print("Event socket closed. Receiver stopped.")

    def thread_event(self):
        event_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        event_socket.settimeout(1.0)
        try:
            event_socket.bind(('0.0.0.0', 30502))
            print(f"UDP Event Receiver initialized. Listening continuously on 192.168.2.10:30502")

            self.running = True
            while self.running:
                try:
                    data, addr = event_socket.recvfrom(1024)
                    if len(data) < 4:
                        print(f"Error: Received truncated data ({len(data)} bytes). Ignoring.")
                        continue

                    response = SomeIPMessage.from_bytes(data)
                    method_id = response.method_id

                    print(f"\n--- Event Received ---")
                    print(f"From: {addr[0]}:{addr[1]}")

                    if (method_id == 0x8200):
                        left_rpm, right_rpm = struct.unpack('!ff', response.payload)
                        print(f"Left RPM: {left_rpm}, Right RPM: {right_rpm}")
                    elif (method_id == 0x8201):
                        aeb_status = struct.unpack('!B', response.payload[:1])[0]
                        print(f"AEB Status: {aeb_status}")
                    elif (method_id == 0x8202):
                        park_status = struct.unpack('!B', response.payload[:1])[0]
                        print(f"Parking Status: {park_status}")
                    elif (method_id == 0x8203):
                        front_dist, back_dist = struct.unpack('!ff', response.payload)
                        print(f"Front Distance: {front_dist}")

                except socket.timeout:
                    continue
                except OSError:
                    break
                except Exception as e:
                    print(f"Error during reception: {e}")
                    continue
        except Exception as e:
            print(f"Error binding socket: {e}")
        finally:
            event_socket.close()
            print("Event socket closed. Receiver stopped.")

    def start(self):
        if not self.event_thread or not self.event_thread.is_alive():
            self.event_thread = threading.Thread(target=self.thread_event, daemon=True)
            self.event_thread.start()
            print("Event listener thread started.")

    def stop(self):
        self.running = False
        if self.event_thread:
            self.event_thread.join()
            print("Event listener thread stopped.")


def client_example():
    client = SomeIPClient()
    
    # 서비스 찾기
    found_services = client.find_service(0xFFFF)
    print(f"0x0100 Service: ip = {Services[0x0100].ip}, port={Services[0x0100].port}")
    print(f"0x0200 Service: ip = {Services[0x0200].ip}, port={Services[0x0200].port}")
    
    for key, value in Methods.items():
        print(f"{key:04x} ; service_id = {value.service.service_id:04x}, ip = {value.service.ip}, port = {value.service.port}, method_id = {value.method_id:04x}, event_group={value.eventgroup_id:04x}")

        serviceID = value.service.service_id
        #if (serviceID==0x0100):
            #client.connect(value.service.ip, value.service.port)
        #    response_data = client.request_service(value.service, value.method_id)
            #print(f"requesst scucess  : {response_data}")
        if (serviceID==0x0200):
            host, port = client.sub_service(value.service.service_id, value.method_id)
           
        #time.sleep(0.5)  # 0.5초 기다림

    #client.get_event()
    client.start()
    while (1):
        print("main thread activate...")
        time.sleep(1)

    receiver.stop()

    #if host: #connect는 TCP보낼때 
    #    client.connect(host, port)
        
        # 현재 속도 얻기
    #    speed_data = client.call_method(0x1234, 0x0001)
    #    speed = struct.unpack('!f', speed_data)[0]
    #    print(f"Current speed: {speed:.2f} km/h")
        
        # 속도 제한 설정
    #    new_limit = struct.pack('!f', 100.0)
    #    client.call_method(0x1234, 0x0002, new_limit)
    #    print("Speed limit updated")
    #

if __name__ == "__main__":
    client_example()




