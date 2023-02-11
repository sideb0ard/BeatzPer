#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>

#include <string>

namespace http {

class HTTPServer {
 public:
  HTTPServer(std::string ip_address, int port);
  ~HTTPServer();
  void Start();

 private:
  int socket_;
  struct sockaddr_in socket_address_;
};

}  // namespace http
