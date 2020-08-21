#include <tello/logger/logger.hpp>
#include <tello/connection/network.hpp>
#include <tello/tello.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include "controller.hpp"


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

int main() {
    LoggerSettings settings{"./log/command_log.log", "./log/video_log.log", "./log/status_log.log"};
    Logger::initialize(settings);

    const bool isConnected = Network::connect();
    assert(isConnected);

    Tello tello(TELLO_IP_ADDRESS);

    future<Response> commandFuture = tello.command();
    commandFuture.wait();
    Response commandResponse = commandFuture.get();

    std::cout << "COMMAND: " << (int) commandResponse.status() << std::endl;

    future<Response> takeoffFuture = tello.takeoff();
    takeoffFuture.wait();
    Response takeoffResponse = takeoffFuture.get();

    std::cout << "TAKE OFF: " << (int) takeoffResponse.status() << std::endl;

    controller(tello);

    return 0;
}

