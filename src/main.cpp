#include <BTrack.h>
#include <sndfile.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <thread>

#include "dirmonitor.h"
#include "httpservrrr.h"

namespace fs = std::filesystem;
constexpr int kLocalBufferLen = 1024;

struct FileTimingInfo {
  FileTimingInfo(std::string path, double bpm,
                 std::vector<double> beat_times_ms)
      : file_path{path}, bpm{bpm}, beat_times_ms{beat_times_ms} {}
  ~FileTimingInfo() = default;
  std::string file_path;
  double bpm;
  std::vector<double> beat_times_ms;
};
std::unordered_map<std::string, FileTimingInfo> g_known_files;

std::vector<double> GetBeatTimesFromAudioFile(std::string full_filename) {
  SNDFILE* snd_file;
  SF_INFO sf_info;
  BTrack beat_tracker;

  std::vector<double> beat_times_ms;

  sf_info.format = 0;

  snd_file = sf_open(full_filename.c_str(), SFM_READ, &sf_info);
  if (!snd_file) {
    std::cerr << "Barfed opening:" << full_filename << " due to "
              << sf_error(snd_file);
  }

  double time_count_ms{0};
  double time_per_frame_ms = 1000. / sf_info.samplerate;
  double local_buffer[kLocalBufferLen]{};
  sf_count_t count;
  while ((count = sf_read_double(snd_file, local_buffer, kLocalBufferLen)) >
         0) {
    time_count_ms += time_per_frame_ms * (count / sf_info.channels);
    beat_tracker.processAudioFrame(local_buffer);
    if (beat_tracker.beatDueInCurrentFrame()) {
      beat_times_ms.push_back(time_count_ms);
    }
  }
  sf_close(snd_file);
  return beat_times_ms;
}

double EstimateBpm(const std::vector<double>& beat_times_ms) {
  std::vector<double> time_diffs;
  for (size_t i = 1; i < beat_times_ms.size(); i++) {
    double time_diff = beat_times_ms[i] - beat_times_ms[i - 1];
    time_diffs.push_back(time_diff);
  }
  double avg_diff = std::accumulate(time_diffs.begin(), time_diffs.end(), 0) /
                    time_diffs.size();
  double bpm_estimate = 1000. * 60 / avg_diff;
  return bpm_estimate;
}

void UpdateKnownFiles(std::string file_path, FileStatus status) {
  if (status == FileStatus::created) {
    std::cout << "OH CREATED! " << file_path << std::endl;
    std::vector<double> beat_times_ms = GetBeatTimesFromAudioFile(file_path);
    double bpm = EstimateBpm(beat_times_ms);

    g_known_files.insert(std::make_pair(
        file_path, FileTimingInfo{file_path, bpm, beat_times_ms}));
  } else if (status == FileStatus::modified) {
    auto file_entry = g_known_files.find(file_path);
    if (file_entry != g_known_files.end()) {
      std::cout << "Oohm found yer file:" << file_path << std::endl;
      file_entry->second.beat_times_ms = GetBeatTimesFromAudioFile(file_path);
      file_entry->second.bpm = EstimateBpm(file_entry->second.beat_times_ms);
    } else {
      std::cerr << "Woof coundae find yer file:" << file_path << std::endl;
    }
  } else if (status == FileStatus::erased) {
    std::cout << "File erased: " << file_path << '\n';
  } else {
    std::cout << "Error! Unknown file status.\n";
  }
}

int main() {
  http::HTTPServer http_server{"0.0.0.0", 8080};
  std::thread http_thread(&http::HTTPServer::Start, http_server);

  DirMonitor dm{"./MusicFiles/", std::chrono::milliseconds(1000)};
  dm.Start(UpdateKnownFiles);

  http_thread.join();
}
