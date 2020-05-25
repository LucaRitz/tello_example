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

extern "C" {
#include <libavcodec/avcodec.h>
}

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

std::string string_to_hex(const unsigned char* input, unsigned int size)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(size * 2);
    for (int i = 0; i < size; i++)
    {
        output.push_back(hex_digits[input[i] >> 4]);
        output.push_back(hex_digits[input[i] & 15]);
    }
    return output;
}

H264Decoder decoder;
bool decodedOne = false;

int main() {
    LoggerSettings settings {"./log/command_log.log", "./log/video_log.log", "./log/status_log.log"};
    Logger::initialize(settings);

    const bool isConnected = Network::connect();
    assert(isConnected);

    Tello tello(TELLO_IP_ADDRESS);

    future<Response> commandResponse = tello.command();
    commandResponse.wait();



    tello.setVideoHandler([](const VideoResponse& video)
                          {
                            if(!decodedOne) {
                                auto* buffer = new unsigned char[video.length()];
                                memcpy(buffer, video.videoFrame(), video.length());
                                std::cout << string_to_hex(video.videoFrame(), video.length()) << std::endl << std::endl;

                                decoder.parse(buffer, video.length());

                                if (decoder.is_frame_available()) {
                                    const AVFrame& frame = decoder.decode_frame();
                                    Mat mat(720, 1280, CV_8UC3, frame.data[0], frame.linesize[0]);
                                    imshow("frame", mat);
                                    waitKey(10);
                                }
                            }
                          });

    future<Response> videoResponse = tello.streamon();
    videoResponse.wait();

    std::chrono::seconds duration(15);
    std::this_thread::sleep_for(duration);

    Logger::get(LoggerType::COMMAND)->info("Test log exe");

    future<Response> videoResponseOff = tello.streamoff();
    videoResponseOff.wait();

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

