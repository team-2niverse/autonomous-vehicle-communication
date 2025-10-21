#include <thread>

#include "someip.hpp"
#include "js.hpp"
#include "led.hpp"
#include "motor.hpp"
#include "hexwatcher.hpp"
#include "reprogramming.hpp"

int main() {
    std::thread(js_run).detach();
    std::thread(led_run).detach();          // need someip
    std::thread(motor_run).detach();        // need someip
    std::thread(hexwatcher_run).detach();   // need someip
    std::thread(reprogramming_run).detach();

    someip_init();

    return 0;
}