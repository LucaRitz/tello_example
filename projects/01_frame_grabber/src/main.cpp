#include <tello/logger/logger.hpp>
#include <tello/connection/network.hpp>
#include <tello/tello.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "lib/shared/H264Decoder.hpp"
#include <chrono>
#include <thread>
#include <frame_grabber.hpp>

#define TELLO_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1

using tello::LoggerSettings;
using tello::Logger;
using tello::Network;
using tello::Tello;
using tello::Response;
using tello::Status;
using std::string;

using std::cout;
using std::endl;
using namespace cv;

#define FRAMES 20

int main() {
    LoggerSettings settings {"./log/command_log.log", "./log/video_log.log", "./log/status_log.log"};
    Logger::initialize(settings);

    const bool isConnected = Network::connect();
    assert(isConnected);

    Tello tello(TELLO_IP_ADDRESS);

    future<Response> commandResponse = tello.command();
    commandResponse.wait();

    future<Response> response = FrameGrabber::instance().observe(tello);
    response.wait();

    int frameCount = 0;
    std::vector<Mat> mats;
    while(frameCount < FRAMES) {
        optional<future<Mat>> frameOpt = FrameGrabber::instance().grab(tello.ip());
        if (frameOpt) {
            frameOpt->wait();

            Mat mat = frameOpt->get();
            if (!mat.empty()) {
                mats.emplace_back(mat);
                frameCount++;
            } else {
                std::cout << "empty frame (timed out)" << std::endl;
            }
        }

        tello.command();
    }

    for(auto& mat : mats) {
        imshow("frame", mat);
        waitKey(0);
    }

    Logger::get(LoggerType::COMMAND)->info("Test log exe");

    future<Response> videoResponseOff = tello.streamoff();
    videoResponseOff.wait();
    std::cout << "Off" << std::endl;

    Network::disconnect();

    return 0;
}

