#include <thread>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "js.hpp"
#include "hexwatcher.hpp"

#define SERVER_IP "192.168.2.20"
#define SERVER_PORT 13401

bool reprogramming_receiveLine(int sock, std::string& response) {
    char buffer[1024] = {0};
    response.clear();
    
    // 서버 응답이 여러 패킷으로 나뉘어 올 수 있으므로, '\n'을 만날 때까지 읽습니다.
    while (true) {
        ssize_t bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            std::cerr << "Error: recv() failed or connection closed by server." << std::endl;
            return false;
        }
        buffer[bytes_received] = '\0';
        response.append(buffer);
        
        // 수신된 문자열에 개행 문자가 있으면 루프를 종료합니다.
        if (response.find('\n') != std::string::npos) {
            break;
        }
    }
    return true;
}

void reprogramming_sendHex(std::string path) {
    std::cout << "sending HEX file..." << std::endl;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Error: Failed to create socket" << std::endl;
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "Error: IP address conversion failed" << std::endl;
        close(sock);
        return;
    }

    std::cout << "Connecting to target board (" << SERVER_IP << ":" << SERVER_PORT << ")..." << std::endl;
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error: Connection failed" << std::endl;
        close(sock);
        return;
    }
    std::cout << "Connection successful" << std::endl;

    std::string response;

    if (!reprogramming_receiveLine(sock, response)) {
        std::cerr << "Error: Cannot receive WELCOME" << std::endl;
        close(sock);
        return;
    }
    std::cout << "Server: " << response;

    std::cout << "Sending START command..." << std::endl;
    std::string start_cmd = "START\n";
    if (send(sock, start_cmd.c_str(), start_cmd.length(), 0) < 0) {
        std::cerr << "Error: Failed to send START command" << std::endl;
        close(sock);
        return;
    }

    // 3. "START"에 대한 서버의 ACK 수신
    if (!reprogramming_receiveLine(sock, response)) {
        close(sock);
        return;
    }
    std::cout << "Server: " << response;
    if (response.find("ACK: START OK") == std::string::npos) {
        std::cerr << "Error: Did not receive START ACK from server." << std::endl;
        close(sock);
        return;
    }
    std::cout << "Handshake successful. Starting file transfer." << std::endl;

    std::ifstream hex(path);
    if (!hex.is_open()) {
        std::cerr << "Error: Cannot open file " << path << std::endl;
        close(sock);
        return;
    }

    // 5. Stop-and-Wait 방식으로 HEX 라인 전송/응답 루프
    std::string line;
    int line_count = 1;
    while (std::getline(hex, line)) {
        if (line.empty()) continue;

        // 5-1. 각 라인 끝에 개행 문자(\n)를 추가하여 전송
        std::string line_with_nl = line + "\n";
        if (send(sock, line_with_nl.c_str(), line_with_nl.length(), 0) < 0) {
            std::cerr << "Error: Failed to send line " << line_count << std::endl;
            hex.close();
            close(sock);
            return;
        }

        // 5-2. 보낸 라인에 대한 서버의 ACK 수신
        if (!reprogramming_receiveLine(sock, response)) {
            hex.close();
            close(sock);
            return;
        }

        // 5-3. 서버 응답 확인
        std::cout << "Line " << line_count++ << ": " << response;
        if (response.find("ACK") == std::string::npos) {
            std::cerr << "Error: Received NACK or unexpected response from server." << std::endl;
            hex.close();
            close(sock);
            return;
        }

        // 5-4. 최종 리부팅 메시지 수신 시 루프 종료
        if (response.find("Rebooting") != std::string::npos) {
            break;
        }
    }

    // std::string line;
    // while (std::getline(hex, line)) {
    //     if (send(sock, line.c_str(), line.length(), 0) < 0) {
    //         std::cerr << "Error: Failed to send" << std::endl;
    //         close(sock);
    //         return;
    //     }
    // }

    hex.close();
    close(sock);

    std::cout << "HEX file transfer completed" << std::endl;
}

void reprogramming_run(void) {
    struct js_event event;
    short btn8 = 0, btn9 = 0;

    while (true) {
        {
            std::unique_lock<std::mutex> lock(g_rep_mtx);
            g_rep_cv.wait(lock, []{ return g_rep_isNew; });
            event = g_rep_ev;
            g_rep_isNew = false;
        }

        switch (event.number) {
            case 8: btn8 = event.value; break;
            case 9: btn9 = event.value; break;
            default: continue;
        }

        if (btn8 && btn9) {
            std::string hexPath = hexwatcher_getPath();
            if (hexPath.empty()) continue;
            std::cout << "HEX path" << hexPath << std::endl;
            reprogramming_sendHex(hexPath);
        }
    }
}