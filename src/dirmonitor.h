#pragma once

#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#include "audio_utils.h"

enum class FileStatus { created, modified, erased };

struct FileTimingInfo {
  FileTimingInfo(std::string path, double bpm,
                 std::vector<double> beat_times_ms)
      : file_path{path}, bpm{bpm}, beat_times_ms{beat_times_ms} {}
  ~FileTimingInfo() = default;
  std::string file_path;
  double bpm;
  std::vector<double> beat_times_ms;
};

class DirMonitor {
 public:
  DirMonitor(std::string file_path,
             std::chrono::duration<int, std::milli> delay)
      : file_path_{file_path}, delay_{delay} {}
  ~DirMonitor() = default;

  void Start();
  void UpdateKnownFiles(std::string file_path, FileStatus status);

 private:
  bool running_{true};
  std::string file_path_;
  std::chrono::duration<int, std::milli> delay_;
  std::unordered_map<std::string, std::filesystem::file_time_type> paths_;

  std::unordered_map<std::string, FileTimingInfo> known_files_;

 private:
  void WriteJsonFile();
};
