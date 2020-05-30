#pragma once

#include <optional>
#include <future>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <chrono>
#include "H264Decoder.hpp"

namespace cv {
    class Mat;
}

namespace tello {
    class Tello;
    class Response;
}

using std::optional;
using std::future;
using std::promise;
using std::unordered_map;
using std::shared_ptr;
using cv::Mat;
using tello::Tello;
using tello::Response;
using ip_address = unsigned long;

namespace grab {
    struct FrameGrabInfo {
        shared_ptr<promise<Mat>> _prom;
        bool _wait;
        time_t _insertTime;
    };
}

class FrameGrabber {
public:
    static FrameGrabber& instance() {
        static FrameGrabber grabber;
        return grabber;
    }

    FrameGrabber(const FrameGrabber&) = delete;
    FrameGrabber(FrameGrabber&&) = delete;
    FrameGrabber& operator=(const FrameGrabber&) = delete;
    FrameGrabber& operator=(FrameGrabber&&) = delete;

    future<Response> observe(Tello& tello);
    optional<future<Mat>> grab(ip_address ip, bool wait = false);

private:
    static time_t currentTime();

    FrameGrabber() = default;

    unordered_map<ip_address, grab::FrameGrabInfo> _data;
    std::mutex _dataMutex;
    H264Decoder _decoder;
};