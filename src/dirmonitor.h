#pragma once

#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#include "bpm_extractor.h"
#include "defjams.h"

class DirMonitor {
 public:
  DirMonitor(std::string file_path,
             std::chrono::duration<int, std::milli> delay)
      : file_path_{file_path}, delay_{delay} {}
  ~DirMonitor() = default;

  void Start();

 private:
  bool running_{true};
  std::string file_path_;
  std::chrono::duration<int, std::milli> delay_;
  std::unordered_map<std::string, std::filesystem::file_time_type> paths_;

  bpm::BPMExtractor bpmx_;
};
