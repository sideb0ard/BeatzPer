#include "httpservrrr.h"

namespace http {

void HTTPServer::Start() {
  using namespace httplib;

  Server svr;

  svr.Get("/hi", [](const Request& req, Response& res) {
    res.set_content("Hello World!", "text/plain");
  });

  svr.Get("/stop", [&](const Request& req, Response& res) { svr.stop(); });

  svr.listen(ip_address_, port_);
}
}  // namespace http
