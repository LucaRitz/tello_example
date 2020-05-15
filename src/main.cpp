#include <tello/logger/logger.hpp>
#include <tello/connection/network.hpp>
#include <tello/tello.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "H264Decoder.hpp"
#include <chrono>
#include <thread>

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

int main() {
    LoggerSettings settings {"./log/command_log.log", "./log/video_log.log", "./log/status_log.log"};
    Logger::initialize(settings);

    const bool isConnected = Network::connect();
    assert(isConnected);

    Tello tello(TELLO_IP_ADDRESS);

    future<Response> commandResponse = tello.command();
    commandResponse.wait();

    future<Response> videoResponse = tello.streamon();
    videoResponse.wait();

    tello.setVideoHandler([](const VideoResponse& video)
        {
            const string& frame = video.videoFrame();
            H264Decoder decoder;
            unsigned char buffer[10000];
            memcpy(buffer, frame.c_str(), frame.size());
            buffer[frame.size()] = '\0';
            decoder.decode(buffer, frame.size());
            decoder.play();
        });

    std::chrono::seconds duration(10);
    std::this_thread::sleep_for(duration);

    Logger::get(LoggerType::COMMAND)->info("Test log exe");

    Network::disconnect();

    // Opencv
    Mat image;
    image = imread("./resources/test.jpg");   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.

    waitKey(0);

    return 0;
}

