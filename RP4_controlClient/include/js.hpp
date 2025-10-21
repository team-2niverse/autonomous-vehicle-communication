#ifndef JS_HPP
#define JS_HPP

#include <mutex>
#include <condition_variable>
#include <linux/joystick.h>

#define JS_DEVICE "/dev/input/js0"

extern struct js_event g_motor_ev;
extern bool g_motor_isNew;
extern std::mutex g_motor_mtx;
extern std::condition_variable g_motor_cv;

extern struct js_event g_led_ev;
extern bool g_led_isNew;
extern std::mutex g_led_mtx;
extern std::condition_variable g_led_cv;

extern struct js_event g_rep_ev;
extern bool g_rep_isNew;
extern std::mutex g_rep_mtx;
extern std::condition_variable g_rep_cv;

void js_run(void);

#endif // JS_HPP