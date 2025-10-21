#ifndef SOMEIP_HPP
#define SOMEIP_HPP

#include <condition_variable>
#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID        0x0100
#define SAMPLE_INSTANCE_ID       0x0001
#define SAMPLE_METHOD_MOTOR_ID   0x0100
#define SAMPLE_METHOD_LED1_ID    0x0101
#define SAMPLE_METHOD_LED2_ID    0x0102
#define SAMPLE_METHOD_LED3_ID    0x0103

extern std::shared_ptr<vsomeip::application> g_someip_app;
extern std::mutex g_someip_mtx;
extern std::condition_variable g_someip_cv;

void someip_init(void);

#endif // SOMEIP_HPP