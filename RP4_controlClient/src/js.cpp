#include <iostream>
#include <thread>

#include <fcntl.h>
#include <unistd.h>

#include "js.hpp"


struct js_event g_motor_ev;
bool g_motor_isNew = false;
std::mutex g_motor_mtx;
std::condition_variable g_motor_cv;

struct js_event g_led_ev;
bool g_led_isNew = false;
std::mutex g_led_mtx;
std::condition_variable g_led_cv;

struct js_event g_rep_ev;
bool g_rep_isNew = false;
std::mutex g_rep_mtx;
std::condition_variable g_rep_cv;


void js_run(void) {
    int fd;
    while ((fd = open(JS_DEVICE, O_RDONLY)) < 0) {
        std::cerr << "Error: Could not open joystick " << JS_DEVICE << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    char name[128];
    if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0)
        snprintf(name, sizeof(name), "Unknown");
    std::cout << "Joystick Name: " << name << std::endl;

    struct js_event event;
    while (read(fd, &event, sizeof(event)) > 0) {
        if (event.type == JS_EVENT_BUTTON && (event.number == 4 || event.number == 5)) {
            {
                std::lock_guard<std::mutex> lock(g_led_mtx);
                g_led_ev = event;
                g_led_isNew = true;
            }
            g_led_cv.notify_one();
        } else if (event.type == JS_EVENT_AXIS && (event.number == 1 || event.number == 3)) {
            {
                std::lock_guard<std::mutex> lock(g_motor_mtx);
                g_motor_ev = event;
                g_motor_isNew = true;
            }
            g_motor_cv.notify_one();
        } else if (event.type == JS_EVENT_BUTTON && (event.number == 8 || event.number == 9)) {
            {
                std::lock_guard<std::mutex> lock(g_rep_mtx);
                g_rep_ev = event;
                g_rep_isNew = true;
            }
            g_rep_cv.notify_one();
        }
    }

    close(fd);
}