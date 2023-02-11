#include "httpservrrr.h"

// a lot taken from:
// https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7

#include <iostream>

namespace http {

HTTPServer::HTTPServer(std::string ip_address, int port) {
  std::cout << "HTTP Server starting on " << ip_address << " port: " << port
            << std::endl;
  socket_address_.sin_family = AF_INET;
  socket_address_.sin_port = htons(port);
  socket_address_.sin_addr.s_addr = inet_addr(ip_address.c_str());

  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ < 0) {
    std::cerr << "Couldn't call socket()!" << std::endl;
  }
  if (bind(socket_, (sockaddr *)&socket_address_, sizeof(socket_address_)) <
      0) {
    std::cerr << "Couldn't create an HTTP Server!" << std::endl;
  }
}

HTTPServer::~HTTPServer() {}
void HTTPServer::Start() {}
}  // end namespace http
