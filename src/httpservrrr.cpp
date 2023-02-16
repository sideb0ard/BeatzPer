#include "httpservrrr.h"

#include <filesystem>
#include <iostream>

namespace http {

void HTTPServer::Start() {
  using namespace httplib;

  Server svr;

  std::cout << "Current path is " << std::filesystem::current_path()
            << '\n';  // (1)
  auto ret = svr.set_mount_point("/", "./www/");
  if (!ret) {
    std::cerr << "WOOF, NAE MOUNT POINT!" << std::endl;
    return;
  }

  std::cout << "HTTP Server started on port " << port_ << std::endl;

  svr.listen(ip_address_, port_);
}
}  // namespace http
