#pragma once

#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

enum class FileStatus { created, modified, erased };

class DirMonitor {
 public:
  DirMonitor(std::string file_path,
             std::chrono::duration<int, std::milli> delay)
      : file_path_{file_path}, delay_{delay} {
    for (auto &file :
         std::filesystem::recursive_directory_iterator(file_path)) {
      std::cout << "Adding " << file.path().string() << std::endl;
      paths_[file.path().string()] = std::filesystem::last_write_time(file);
    }
  }
  ~DirMonitor() = default;

  void Start(const std::function<void(std::string, FileStatus)> &action) {
    while (running_) {
      std::this_thread::sleep_for(delay_);

      auto it = paths_.begin();
      while (it != paths_.end()) {
        if (!std::filesystem::exists(it->first)) {
          action(it->first, FileStatus::erased);
          it = paths_.erase(it);
        } else {
          it++;
        }
      }

      for (auto &file :
           std::filesystem::recursive_directory_iterator(file_path_)) {
        auto current_file_last_write_time =
            std::filesystem::last_write_time(file);
        if (!paths_.contains(file.path().string())) {
          paths_[file.path().string()] = current_file_last_write_time;
          action(file.path().string(), FileStatus::created);
        } else {
          if (paths_[file.path().string()] != current_file_last_write_time) {
            paths_[file.path().string()] = current_file_last_write_time;
            action(file.path().string(), FileStatus::modified);
          }
        }
      }
    }
  }

 private:
  bool running_{true};
  std::string file_path_;
  std::chrono::duration<int, std::milli> delay_;
  std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
};
