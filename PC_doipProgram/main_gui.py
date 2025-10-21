# 프로그램의 시작점이자 사용자 인터페이스 (GUI) 담당
# doip_client.py에서 print를 없애고 GUI를 얹음

# ----- doip_client.py 리팩토링 -----
# gui_doip_client.py: doip_client의 로직을 클래스 형태로 재구성
# main_gui.py: doip_client의 입출력을 GUI로 재구성

import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox, simpledialog
import threading
import queue
import time
import doip_messages
from gui_doip_client import DoIPClient

SERVER_HOST = '192.168.26.123'
SERVER_PORT = 13400

class App:
    def __init__(self, root):
        self.root = root
        self.root.title("DoIP Diagnostic Tool")
        self.root.geometry("950x500")

        self.log_queue = queue.Queue()
        self.client = DoIPClient(SERVER_HOST, SERVER_PORT, log_callback=self.queue_log)
        self.current_session = "DEFAULT"

        self.create_widgets()
        self.root.after(100, self.process_log_queue)
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)

    def create_widgets(self):
        conn_frame = tk.Frame(self.root, pady=5)
        conn_frame.pack(fill=tk.X, padx=10, pady=5)
        self.connect_button = tk.Button(conn_frame, text="Connect", command=self.connect, width=12)
        self.connect_button.pack(side=tk.LEFT)
        self.disconnect_button = tk.Button(conn_frame, text="Disconnect", command=self.disconnect, state=tk.DISABLED, width=12)
        self.disconnect_button.pack(side=tk.LEFT, padx=5)
        self.log_area = scrolledtext.ScrolledText(self.root, wrap=tk.WORD, state=tk.DISABLED, font=("Consolas", 10))
        self.log_area.pack(padx=10, pady=5, expand=True, fill=tk.BOTH)
        self.status_var = tk.StringVar()
        self.status_var.set("Not connected.")
        status_label = tk.Label(self.root, textvariable=self.status_var, bd=1, relief=tk.SUNKEN, anchor=tk.W)
        status_label.pack(side=tk.BOTTOM, fill=tk.X)
        cmd_container = tk.Frame(self.root, pady=5)
        cmd_container.pack(fill=tk.X, padx=5, pady=5, side=tk.BOTTOM)
        cmd_container.columnconfigure((0, 1, 2, 3), weight=1)
        session_frame = ttk.LabelFrame(cmd_container, text=" Session Control ")
        session_frame.grid(row=0, column=0, padx=5, sticky="nsew")
        tk.Button(session_frame, text="Default Session", command=lambda: self.run_task(self.stop_extended_session)).pack(fill=tk.X, padx=5, pady=3)
        tk.Button(session_frame, text="Extended Session", command=lambda: self.run_task(self.start_extended_session)).pack(fill=tk.X, padx=5, pady=3)
        read_frame = ttk.LabelFrame(cmd_container, text=" Read Data Identifiers ")
        read_frame.grid(row=0, column=1, padx=5, sticky="nsew")
        read_frame.columnconfigure((0, 1), weight=1)
        read_buttons = {"Read VIN": doip_messages.REQ_READ_VIN, "Read ECU HW": doip_messages.REQ_READ_ECU_HW, "Read ECU SW": doip_messages.REQ_READ_ECU_SW, "Read ECU NUM": doip_messages.REQ_READ_ECU_NUM, "Read System ID": doip_messages.REQ_READ_ID, "Read Ultrasound": doip_messages.REQ_READ_ULTRASOUND, "Read ToF": doip_messages.REQ_READ_TOF, "Read RPM": doip_messages.REQ_READ_RPM, "Read LED": doip_messages.REQ_READ_LED, "Read AEB Offset": doip_messages.REQ_READ_OFFSET, }
        row, col = 0, 0
        for text, msg in read_buttons.items():
            btn_cmd = lambda m=msg, t=text: self.send_request(m, t)
            tk.Button(read_frame, text=text, command=lambda c=btn_cmd: self.run_task(c)).grid(row=row, column=col, padx=5, pady=3, sticky="ew")
            col += 1
            if col > 1:
                col = 0
                row += 1
        write_frame = ttk.LabelFrame(cmd_container, text=" Write Data Identifiers ")
        write_frame.grid(row=0, column=2, padx=5, sticky="nsew")
        tk.Button(write_frame, text="Write AEB Offset", command=self.handle_write_offset).pack(fill=tk.X, padx=5, pady=3)
        dtc_frame = ttk.LabelFrame(cmd_container, text=" DTC Functions ")
        dtc_frame.grid(row=0, column=3, padx=5, sticky="nsew")
        dtc_buttons = { "Read DTC Count": doip_messages.REQ_READ_DTC_CNT, "Read DTC List": doip_messages.REQ_READ_DTC_LIST, "Clear DTC": doip_messages.REQ_CLEAR_DTC, "Activate DTC Rec": doip_messages.REQ_SET_DTC_ACT, "Deactivate DTC Rec": doip_messages.REQ_SET_DTC_DACT, }
        for text, msg in dtc_buttons.items():
            btn_cmd = lambda m=msg, t=text: self.send_request(m, t)
            tk.Button(dtc_frame, text=text, command=lambda c=btn_cmd: self.run_task(c)).pack(fill=tk.X, padx=5, pady=3)

    def format_dict_for_log(self, data, indent=1):
        """ 딕셔너리나 리스트를 보기 좋게 여러 줄의 문자열로 변환하고, 숫자는 16진수로 포맷합니다. """
        if not isinstance(data, (dict, list)):
            return str(data)
        
        indent_str = "    " * indent
        lines = []
        
        if isinstance(data, dict):
            for key, value in data.items():
                value_str = ""
                if isinstance(value, int):
                    if key == 'did':
                        value_str = f"0x{value:04X}"
                    else:
                        value_str = f"0x{value:02X}"
                else:
                    value_str = self.format_dict_for_log(value, indent + 1)
                
                if isinstance(value, (dict, list)):
                    lines.append(f"{indent_str}- {key}:\n{value_str}")
                else:
                    lines.append(f"{indent_str}- {key}: {value_str}")

        elif isinstance(data, list):
            for i, item in enumerate(data):
                item_str = self.format_dict_for_log(item, indent + 1)
                lines.append(f"{indent_str}[{i}]:\n{item_str}")

        return "\n".join(lines)

    def queue_log(self, message, level='INFO'):
        self.log_queue.put((str(message), level))

    def process_log_queue(self):
        try:
            while True:
                message, level = self.log_queue.get_nowait()
                if level == 'STATUS':
                    self.status_var.set(message)
                else:
                    self.log_area.config(state=tk.NORMAL)
                    self.log_area.insert(tk.END, f"[{time.strftime('%H:%M:%S')}] {message}\n")
                    self.log_area.config(state=tk.DISABLED)
                    self.log_area.see(tk.END)
        except queue.Empty:
            pass
        self.root.after(100, self.process_log_queue)
        
    def run_task(self, target_func, *args):
        task_thread = threading.Thread(target=target_func, args=args, daemon=True)
        task_thread.start()
    
    def connect(self):
        self.run_task(self._connect_task)
        
    def _connect_task(self):
        self.queue_log("-" * 60)
        success, message = self.client.connect()
        if success:
            self.connect_button.config(state=tk.DISABLED)
            self.disconnect_button.config(state=tk.NORMAL)
            self.status_var.set(f"Connected to {SERVER_HOST} [Default Session]")

    def disconnect(self):
        self.run_task(self._disconnect_task)

    def _disconnect_task(self):
        self.queue_log("-" * 60)
        self.client.disconnect()
        self.current_session = "DEFAULT"
        self.connect_button.config(state=tk.NORMAL)
        self.disconnect_button.config(state=tk.DISABLED)
        self.status_var.set("Not connected.")
        
    def send_request(self, message, request_name):
        self.queue_log("-" * 60)
        self.queue_log(f"--> {request_name} 요청...")
        result = self.client.send_diagnostic_request(message)
        self.queue_log(f"<-- 응답:\n{self.format_dict_for_log(result)}")

    def start_extended_session(self):
        self.queue_log("-" * 60)
        if self.current_session == "EXTENDED":
            self.queue_log("이미 Extended Session 입니다.")
            return
        self.queue_log("--> Extended Session으로 변경 요청...")
        result = self.client.send_diagnostic_request(doip_messages.REQ_SESSION_EXTENDED)
        self.queue_log(f"<-- 응답:\n{self.format_dict_for_log(result)}")
        if result and result.get('status') == 'success':
            self.client.start_tester_present_thread()
            self.current_session = "EXTENDED"
            self.status_var.set(f"Connected to {SERVER_HOST} [Extended Session]")

    def stop_extended_session(self):
        self.queue_log("-" * 60)
        if self.current_session == "DEFAULT":
            self.queue_log("이미 Default Session 입니다.")
            return
        self.queue_log("--> Default Session으로 변경 요청...")
        self.client.stop_tester_present_thread()
        result = self.client.send_diagnostic_request(doip_messages.REQ_SESSION_DEFAULT)
        self.queue_log(f"<-- 응답:\n{self.format_dict_for_log(result)}")
        if result and result.get('status') == 'success':
            self.current_session = "DEFAULT"
            self.status_var.set(f"Connected to {SERVER_HOST} [Default Session]")
    
    def handle_write_offset(self):
        value = simpledialog.askinteger("Input", "Enter new offset value (0-65535):",
                                        parent=self.root, minvalue=0, maxvalue=65535)
        if value is not None:
            message = doip_messages.create_req_write_offset(value)
            self.run_task(self.send_request, message, "Write AEB Offset")
        else:
            self.queue_log("AEB Offset 값 변경이 취소되었습니다.")

    def on_closing(self):
        if messagebox.askokcancel("Quit", "Do you want to quit?"):
            self.run_task(self.client.disconnect)
            self.root.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()