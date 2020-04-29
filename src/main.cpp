#include <tello/logger/logger.hpp>
#include <tello/connection/network.hpp>
#include <tello/command_factory.hpp>
#include <tello/tello.hpp>
#include <tello/command.hpp>
#include <tello/response.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#define TELLO_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1

using tello::LoggerSettings;
using tello::Logger;
using tello::Network;
using tello::CommandFactory;
using tello::Command;
using tello::CommandType;
using tello::Tello;
using tello::Response;
using tello::Status;
using std::string;
using ComPtr = std::optional<std::unique_ptr<Command>>;

using std::cout;
using std::endl;
using namespace cv;

int main() {
    LoggerSettings settings {"./log/command_log.log", "./log/video_log.log", "./log/status_log.log"};
    Logger::initialize(settings);

    const bool isConnected = Network::connect();
    assert(isConnected);

    Tello tello(TELLO_IP_ADDRESS);

    ComPtr command = CommandFactory::build(CommandType::COMMAND);
    std::unique_ptr<Response> responseCommand = tello.exec(*(command.value()));

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
