#pragma once

#include <httplib.h>

#include <string>

namespace http {

class HTTPServer {
 public:
  HTTPServer(std::string ip_address, int port, std::string mount_point)
      : ip_address_{ip_address}, port_{port}, mount_point_{mount_point} {}
  ~HTTPServer() = default;

  void Start();

 private:
  std::string ip_address_;
  int port_;
  std::string mount_point_;
};

}  // namespace http
