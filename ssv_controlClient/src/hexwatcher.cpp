#include <thread>
#include <iostream>
#include <filesystem>
#include <vsomeip/vsomeip.hpp>
#include <sys/inotify.h>
#include <unistd.h>

#include "someip.hpp"

#define EVENT_SIZE      ( sizeof (struct inotify_event) )
#define BUFFER_LEN      ( 1024 * ( EVENT_SIZE + 16 ) )

std::string g_hex_path;
std::mutex g_hex_mtx;

std::string hexwatcher_getPath(void) {
    std::lock_guard<std::mutex> lock(g_hex_mtx);
    return g_hex_path;
}

void hexwatcher_updatePath(const std::string& dir) {
    try {
        if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir)) {
            std::lock_guard<std::mutex> lock(g_hex_mtx);
            g_hex_path.clear();
            std::cerr << "cannnot find " << dir << std::endl;
            return;
        }

        std::filesystem::path latest_path;
        auto latest_time = std::filesystem::file_time_type::min();

        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".hex") {
                auto entry_time = std::filesystem::last_write_time(entry);
                if (entry_time > latest_time) {
                    latest_time = entry_time;
                    latest_path = entry.path();
                }
            }
        }

        {
            std::lock_guard<std::mutex> lock(g_hex_mtx);
            if (latest_path.empty())
                g_hex_path.clear();
            else
                g_hex_path = latest_path.string();
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "filesystem error: " << e.what() << std::endl;
    }
}

void hexwatcher_run(void) {
    {
        std::unique_lock<std::mutex> init_lock(g_someip_mtx);
        g_someip_cv.wait(init_lock);
    }

    std::string dir = std::string(std::getenv("HOME")) + "/ota/hex";
    int fd;
    while ((fd = inotify_init()) < 0) {
        std::cerr << "Error: inotify_init()" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    int wd = inotify_add_watch(fd, dir.c_str(), IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM);
    if (wd < 0) {
        std::cerr << "Error: inotify_add_watch()" << std::endl;
        return;
    }

    // hexwatcher_updatePath(dir);

    char buffer[BUFFER_LEN];
    while (true) {
        // int len = read(fd, buffer, BUFFER_LEN);
        hexwatcher_updatePath(dir);
        std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
        request->set_service(SAMPLE_SERVICE_ID);
        request->set_instance(SAMPLE_INSTANCE_ID);
        request->set_method(SAMPLE_METHOD_LED3_ID);

        std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
        std::vector<vsomeip::byte_t> its_payload_data;
        std::string hexPath = hexwatcher_getPath();
        if (hexPath.empty()) {
            // someip light off
            its_payload_data.push_back(0);
            std::cout << "Hexwatcher: no hex file" << std::endl;
        } else {
            // someip light on
            its_payload_data.push_back(1);
            std::cout << "Hexwatcher: hex file found - " << hexPath << std::endl;
        }
        its_payload->set_data(its_payload_data);
        request->set_payload(its_payload);

        g_someip_app->send(request);

        read(fd, buffer, BUFFER_LEN);
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}