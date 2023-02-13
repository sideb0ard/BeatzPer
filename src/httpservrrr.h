#pragma once

#include <httplib.h>

#include <string>

namespace http {

class HTTPServer {
 public:
  HTTPServer(std::string ip_address, int port)
      : ip_address_{ip_address}, port_{port} {}
  ~HTTPServer() = default;

  void Start();

 private:
  std::string ip_address_;
  int port_;
};

}  // namespace http
