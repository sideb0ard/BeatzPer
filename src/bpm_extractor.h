#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "defjams.h"

namespace bpm {

struct FileTimingInfo {
  FileTimingInfo(std::string path, double bpm,
                 std::vector<double> beat_times_ms)
      : file_path{path}, bpm{bpm}, beat_times_ms{beat_times_ms} {}
  ~FileTimingInfo() = default;
  std::string file_path;
  double bpm;
  std::vector<double> beat_times_ms;
};

std::vector<double> GetBeatTimesFromAudioFile(std::string full_filename);
double EstimateBpm(const std::vector<double>& beat_times_ms);

class BPMExtractor {
 public:
  BPMExtractor();
  ~BPMExtractor() = default;
  void UpdateKnownFiles(std::string file_path, FileStatus status);

 private:
  std::unordered_map<std::string, FileTimingInfo> known_files_;
};

}  // namespace bpm
