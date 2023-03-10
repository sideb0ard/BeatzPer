#include "audio_utils.h"

#include <BTrack.h>
#include <sndfile.h>

#include <iostream>
#include <numeric>

constexpr int kLocalBufferLen = 1024;

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
