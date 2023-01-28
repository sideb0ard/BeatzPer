#include <sndfile.h>

#include <filesystem>
#include <iostream>
// #include <fstream>
namespace fs = std::filesystem;

constexpr int kLocalBufferLen = 1024;

const std::string kSamplePath = "galcher.aiff";

int main() {
  SNDFILE* snd_file;
  SF_INFO sf_info;
  std::string full_filename = fs::current_path().string() + "/" + kSamplePath;

  sf_info.format = 0;

  snd_file = sf_open(full_filename.c_str(), SFM_READ, &sf_info);
  if (!snd_file) {
    std::cerr << "Barfed opening:" << full_filename << " due to "
              << sf_error(snd_file);
  }

  std::cout << "OPENED FILE YO - GOT CHANNEL:" << sf_info.channels
            << " aDN THIS MANY FRAMES:" << sf_info.frames << std::endl;

  double local_buffer[kLocalBufferLen]{};
  sf_count_t count;
  while ((count = sf_read_double(snd_file, local_buffer, kLocalBufferLen)) >
         0) {
    // for (int i = 0; i < count; i++) buffer.push_back(local_buffer[i]);
    std::cout << "Read:" << count << std::endl;
  }
  sf_close(snd_file);
}
