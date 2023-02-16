#include <chrono>
#include <string>
#include <thread>

#include "defjams.h"
#include "dirmonitor.h"
#include "httpservrrr.h"

int main() {
  http::HTTPServer http_server{"0.0.0.0", 8080, kMountPoint};
  std::thread http_thread(&http::HTTPServer::Start, http_server);

  DirMonitor dm{{kMountPoint + "/" + kMusicFilesLocation + "/"},
                std::chrono::milliseconds(1000)};
  dm.Start();

  http_thread.join();
}
