#include <chrono>
#include <thread>

#include "bpm_extractor.h"
#include "dirmonitor.h"
#include "httpservrrr.h"

int main() {
  http::HTTPServer http_server{"0.0.0.0", 8080};
  std::thread http_thread(&http::HTTPServer::Start, http_server);

  DirMonitor dm{"./MusicFiles/", std::chrono::milliseconds(1000)};
  dm.Start();

  http_thread.join();
}
