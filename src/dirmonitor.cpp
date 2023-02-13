#include "dirmonitor.h"

#include <thread>

namespace {
bool IsHiddenFile(const std::filesystem::directory_entry direntry) {
  auto fname = direntry.path().filename().string();
  if (fname.size() > 0 && fname[0] == '.') return true;
  return false;
}
}  // end namespace

void DirMonitor::Start() {
  while (running_) {
    std::this_thread::sleep_for(delay_);

    auto it = paths_.begin();
    while (it != paths_.end()) {
      if (!std::filesystem::exists(it->first)) {
        bpmx_.UpdateKnownFiles(it->first, FileStatus::erased);
        it = paths_.erase(it);
      } else {
        it++;
      }
    }

    for (auto& file :
         std::filesystem::recursive_directory_iterator(file_path_)) {
      if (!std::filesystem::is_regular_file(file.path()) || IsHiddenFile(file))
        continue;
      auto current_file_last_write_time =
          std::filesystem::last_write_time(file);
      if (!paths_.contains(file.path().string())) {
        paths_[file.path().string()] = current_file_last_write_time;
        bpmx_.UpdateKnownFiles(file.path().string(), FileStatus::created);
      } else {
        if (paths_[file.path().string()] != current_file_last_write_time) {
          paths_[file.path().string()] = current_file_last_write_time;
          bpmx_.UpdateKnownFiles(file.path().string(), FileStatus::modified);
        }
      }
    }
  }
}
