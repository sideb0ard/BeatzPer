#pragma once

#include <string>
#include <vector>

std::vector<double> GetBeatTimesFromAudioFile(std::string full_filename);
double EstimateBpm(const std::vector<double>& beat_times_ms);
