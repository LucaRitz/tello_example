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

   /* future<Response> commandResponse = tello.command();
    commandResponse.wait();

    future<Response> videoResponse = tello.streamon();
    videoResponse.wait();*/

    string frame("ÙkgÞµ\u0006±`»\u001F²®\u0003u¦i8ãŠ\u0017½gÿ\u00AD§ø|ýü$H\\Ñ¼z\u0011í\u001C>%åøÚ\u0007fît\u0001OEÐ`ês—Kç\u008F\u001FóI£O|dµC±í º3sUÑ-…—\u0081Ø_tˆgñ÷Ê1tR^A,ª%ÞŠ^ºcª#V ×\u001C—Y¤\u001D!<š0Æý89\u0011\u001Daç\u0006½€É\u0090\u0016+Ñk\u008DÎ‰)“€zŒ+¶ðø–Æ\u0011Rm(Æmv/š\u001A1¥išÿEÎîÑ¥í\"Ø\"ü \u0003;.ìºï\u009D?\n"
                 "›9Ê9¡²µ\u00ADò3~\u007F\u0010ÈP\n"
                 ";‰ˆ\t\u0090wÜà³\n"
                 "óÌrð’Ùþ;7\u0081)o5\u001AO-ap–N=y\u0017‹\u0019þ÷,\u0015a");
    H264Decoder decoder;
    unsigned char buffer[10000];
    memcpy(buffer, frame.c_str(), frame.size());
    decoder.decode(buffer, frame.size());
    decoder.play();

    tello.setVideoHandler([](const VideoResponse& video)
        {
            const unsigned char* frame = video.videoFrame();
            H264Decoder decoder;
            auto* buffer = new unsigned char[video.length()];
            memcpy(buffer, frame, video.length());
            decoder.decode(buffer, video.length());
            delete[] buffer;
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

