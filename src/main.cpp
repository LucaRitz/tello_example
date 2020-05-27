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
ConverterRGB24 converter;
bool decodedOne = false;
std::vector<Mat> mats;

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
                              //std::cout << string_to_hex(video.videoFrame(), video.length()) << std::endl << std::endl;

                              std::cout << "Start decoding " << std::endl;
                              int read = decoder.parse(video.videoFrame(), video.length());
                              std::cout << "Has read " << read << std::endl;
                              decoder.parse(video.videoFrame() + video.length(), 0); // Signal end of file

                              if (decoder.is_frame_available()) {
                                  std::cout << "Frame is available" << std::endl;
                                  try {
                                      std::cout << "Decode frame" << std::endl;
                                      const AVFrame& frame = decoder.decode_frame();
                                      std::cout << "Frame decoded" << std::endl;
                                      unsigned char* buffer2 = new unsigned char[video.length()];
                                      std::cout << "Convert" << std::endl;
                                      const AVFrame& rgbFrame = converter.convert(frame, buffer2);
                                      std::cout << "Frame converted" << std::endl;
                                      AVFrame* frameCopy = av_frame_alloc();
                                      av_frame_copy(frameCopy, &rgbFrame);

                                      std::cout << "---- Frame ----" << std::endl;
                                      std::cout << "Height: " << rgbFrame.height << std::endl;
                                      std::cout << "Width: " << rgbFrame.width << std::endl;
                                      std::cout << "Data Ptr: " << rgbFrame.data[0] << std::endl;
                                      std::cout << "Linesize: " << rgbFrame.linesize[0] << std::endl;

                                      Mat mat(rgbFrame.height, rgbFrame.width, CV_8UC3, rgbFrame.data[0],
                                              rgbFrame.linesize[0]);
                                      imshow("frame", mat);
                                  } catch(...) {
                                      std::cout << "Exception occured" << std::endl;
                                  }

                                 /* mats.emplace_back(Mat(rgbFrame.height, rgbFrame.width, CV_8UC3, frameCopy->data[0],
                                                        rgbFrame.linesize[0]));*/
                              }
                          });

    future<Response> videoResponse = tello.streamon();
    videoResponse.wait();

    std::chrono::seconds duration(15);
    std::this_thread::sleep_for(duration);

    for(auto& mat : mats) {
        std::cout << "Show Mat" << std::endl;
        imshow("frame", mat);
        waitKey(10);
    }

    Logger::get(LoggerType::COMMAND)->info("Test log exe");

    future<Response> videoResponseOff = tello.streamoff();
    videoResponseOff.wait();
    std::cout << "Off" << std::endl;

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

