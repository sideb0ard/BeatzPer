#include "dirmonitor.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <thread>

#include "defjams.h"

using json = nlohmann::json;

namespace {
bool IsHiddenFile(const std::filesystem::directory_entry direntry) {
  auto fname = direntry.path().filename().string();
  if (fname.size() > 0 && fname[0] == '.') return true;
  return false;
}

const std::string json_temp_name{"files_temp.json"};
const std::string json_file_name{"www/files_info.json"};

const std::string host_name{"http://localhost:8080"};

}  // end namespace

void DirMonitor::Start() {
  while (running_) {
    std::this_thread::sleep_for(delay_);

    auto it = paths_.begin();
    while (it != paths_.end()) {
      if (!std::filesystem::exists(it->first)) {
        UpdateKnownFiles(it->first, FileStatus::erased);
        it = paths_.erase(it);
      } else {
        it++;
      }
    }

    bool should_update_json{false};
    for (auto& file :
         std::filesystem::recursive_directory_iterator(file_path_)) {
      if (!std::filesystem::is_regular_file(file.path()) || IsHiddenFile(file))
        continue;
      auto current_file_last_write_time =
          std::filesystem::last_write_time(file);
      if (!paths_.contains(file.path().string())) {
        paths_[file.path().string()] = current_file_last_write_time;
        UpdateKnownFiles(file.path().string(), FileStatus::created);
        should_update_json = true;
      } else {
        if (paths_[file.path().string()] != current_file_last_write_time) {
          paths_[file.path().string()] = current_file_last_write_time;
          UpdateKnownFiles(file.path().string(), FileStatus::modified);
          should_update_json = true;
        }
      }
    }
    if (should_update_json) {
      std::cout << "CH-ch-changes detected! Regenerating JSON listing..\n";
      WriteJsonFile();
    }
  }
}

void DirMonitor::UpdateKnownFiles(std::string file_path, FileStatus status) {
  // sorry for the magic number 2! its for the two '/' between each component
  std::string normalized_file_path =
      file_path.substr(kMountPoint.size() + kMusicFilesLocation.size() + 2);
  std::cout << "NORMALIZED PATH:" << normalized_file_path << std::endl;
  if (status == FileStatus::created) {
    std::cout << "OH CREATED! " << file_path << std::endl;
    std::vector<double> beat_times_ms = GetBeatTimesFromAudioFile(file_path);
    double bpm = EstimateBpm(beat_times_ms);

    known_files_.insert(std::make_pair(
        file_path, FileTimingInfo{normalized_file_path, bpm, beat_times_ms}));
  } else if (status == FileStatus::modified) {
    auto file_entry = known_files_.find(file_path);
    if (file_entry != known_files_.end()) {
      std::cout << "Oohm found yer file:" << file_path << std::endl;
      file_entry->second.beat_times_ms = GetBeatTimesFromAudioFile(file_path);
      file_entry->second.bpm = EstimateBpm(file_entry->second.beat_times_ms);
    } else {
      std::cerr << "Woof! coundae find yer file:" << file_path << std::endl;
    }
  } else if (status == FileStatus::erased) {
    std::cout << "File erased: " << file_path << '\n';
  } else {
    std::cout << "Error! Unknown file status.\n";
  }
}

void DirMonitor::WriteJsonFile() {
  std::cout << "WRITING JSON!\n";
  std::ofstream out_file{json_temp_name};

  json j;
  j["file_info"] = {};

  for (const auto& [k, v] : known_files_) {
    std::cout << k << std::endl;
    std::cout << "MOUTPSIX:" << kMountPoint.size() << std::endl;
    std::string url = host_name + "/" + kMusicFilesLocation + "/" + v.file_path;
    std::cout << "URL IS " << url << std::endl;
    json f = {{"name", v.file_path},
              {"link", url},
              {"bpm", v.bpm},
              {"beat_time_ms", v.beat_times_ms}};
    j["file_info"].push_back(f);
  }

  out_file << std::setw(4) << j << std::endl;
  out_file.close();

  std::filesystem::rename(json_temp_name, json_file_name);
}
