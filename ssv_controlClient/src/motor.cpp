#include <thread>

#include "js.hpp"
#include "someip.hpp"

void motor_run(void) {
    {
        std::unique_lock<std::mutex> init_lock(g_someip_mtx);
        g_someip_cv.wait(init_lock);
    }

    struct js_event event;
    short left_axis = 0, right_axis = 0;

    while (true) {
        {
            std::unique_lock<std::mutex> lock(g_motor_mtx);
            g_motor_cv.wait(lock, []{ return g_motor_isNew; });
            event = g_motor_ev;
            g_motor_isNew = false;
        }

        switch (event.number) {
            case 1:
                if (-3276 < event.value && event.value < 3276) {
                    if (left_axis == 0) continue;
                    else event.value = 0;
                }
                left_axis = event.value;
                break;
            case 3:
                if (-3276 < event.value && event.value < 3276) {
                    if (right_axis == 0) continue;
                    else event.value = 0;
                }
                right_axis = event.value;
                break;
            default:
                continue;
        }

        unsigned char dir = (left_axis > 0) ? 1 : 0; 
        int drive = (left_axis < 0) ? (-static_cast<int>(left_axis))>>7 : (static_cast<int>(left_axis))>>7;
        int diff = static_cast<int>(drive * (static_cast<float>(right_axis)/32767));

        unsigned char dutyLeft = 0;
        if (drive + diff > 255) dutyLeft = 255;
        else if (drive + diff > 0) dutyLeft = static_cast<unsigned char>(drive + diff);
        
        unsigned char dutyRight = 0;
        if (drive - diff > 255) dutyRight = 255;
        else if (drive - diff > 0) dutyRight = static_cast<unsigned char>(drive - diff);

        std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
        request->set_service(SAMPLE_SERVICE_ID);
        request->set_instance(SAMPLE_INSTANCE_ID);
        request->set_method(SAMPLE_METHOD_MOTOR_ID);

        std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
        std::vector<vsomeip::byte_t> its_payload_data;
        its_payload_data.push_back(dir);
        its_payload_data.push_back(dutyLeft);
        its_payload_data.push_back(dutyRight);
        its_payload->set_data(its_payload_data);
        request->set_payload(its_payload);

        g_someip_app->send(request);
    }
}