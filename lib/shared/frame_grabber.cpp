#include "frame_grabber.hpp"

#include <tello/tello.hpp>
#include "opencv2/opencv.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
}

using cv::Mat;

future<Response> FrameGrabber::observe(tello::Tello& tello) {

    tello.setVideoHandler([ip = tello.ip(), this](const VideoResponse& video) {
        _dataMutex.lock();
        auto dataIterator = _data.find(ip);
        if (dataIterator != _data.end()) {
            _decoder.parse(video.videoFrame(), video.length());
            _decoder.parse(video.videoFrame() + video.length(), 0); // Signal end of file

            if (_decoder.is_frame_available()) {
                try {
                    const AVFrame& frame = _decoder.decode_frame();

                    if (frame.key_frame) {
                        cv::Size actual_size(1024, 720);
                        cv::Size half_size(512, 360);

                        cv::Mat y(actual_size, CV_8UC1, frame.data[0]);
                        cv::Mat u(half_size, CV_8UC1, frame.data[1]);
                        cv::Mat v(half_size, CV_8UC1, frame.data[2]);

                        cv::Mat u_resized, v_resized;
                        cv::resize(u, u_resized, actual_size, 0, 0, cv::INTER_NEAREST); //repeat u values 4 times
                        cv::resize(v, v_resized, actual_size, 0, 0, cv::INTER_NEAREST); //repeat v values 4 times

                        cv::Mat yuv;

                        std::vector<cv::Mat> yuv_channels = { y, u_resized, v_resized };
                        cv::merge(yuv_channels, yuv);

                        cv::Mat bgr;
                        cv::cvtColor(yuv, bgr, cv::COLOR_YUV2BGR);

                        dataIterator->second._prom->set_value(bgr);
                        _data.erase(ip);
                    } else {
                        if (!dataIterator->second._wait) {
                            time_t now = currentTime();
                            if (now - dataIterator->second._insertTime >= 3) {
                                dataIterator->second._prom->set_value(Mat{});
                                _data.erase(ip);
                            }
                        }
                    }
                } catch(...) {
                    std::cout << "Exception occured" << std::endl;
                }
            }
        }
        _dataMutex.unlock();
    });

    return tello.streamon();
}

optional<future<Mat>> FrameGrabber::grab(ip_address ip, bool wait) {
    auto dataIterator = _data.find(ip);
    if (dataIterator != _data.end()) {
        return std::nullopt;
    }
    promise<Mat> prom{};
    future<Mat> fut_prom = std::move(prom.get_future());

    _dataMutex.lock();
    _data[ip] = grab::FrameGrabInfo{std::make_shared<promise<Mat>>(std::move(prom)), wait, currentTime()};
    _dataMutex.unlock();

    return std::make_optional<future<Mat>>(std::move(fut_prom));
}

time_t FrameGrabber::currentTime() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(now).count();
}