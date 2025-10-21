#include <thread>
#include <linux/joystick.h>
#include <vsomeip/vsomeip.hpp>

#include "js.hpp"
#include "someip.hpp"

void led_run(void) {
    {
        std::unique_lock<std::mutex> init_lock(g_someip_mtx);
        g_someip_cv.wait(init_lock);
    }

    vsomeip::method_t method;
    struct js_event event;

    while (true) {
        {
            std::unique_lock<std::mutex> lock(g_led_mtx);
            g_led_cv.wait(lock, []{ return g_led_isNew; });
            event = g_led_ev;
            g_led_isNew = false;
        }

        switch (event.number) {
            case 4: method = SAMPLE_METHOD_LED1_ID; break;
            case 5: method = SAMPLE_METHOD_LED2_ID; break;
            default: continue;
        }

        std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
        request->set_service(SAMPLE_SERVICE_ID);
        request->set_instance(SAMPLE_INSTANCE_ID);
        request->set_method(method);

        std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
        std::vector<vsomeip::byte_t> its_payload_data;
        its_payload_data.push_back(event.value);
        its_payload->set_data(its_payload_data);
        request->set_payload(its_payload);

        g_someip_app->send(request);
    }
}
