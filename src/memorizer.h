#ifndef MEMORIZER_H
#define MEMORIZER_H

#include <vector>
//these should add up to one
constexpr float AMPLITUDE_WEIGHT = .25;
constexpr float PEAK_FREQUENCY_WEIGHT = .25;
constexpr float BRIGHTNESS_WEIGHT = .25;
constexpr float TIME_WEIGHT = .25;

//features of a sound
struct Feature {
    float ampltiude;
    float peakFrequency;
    float brightness;
    long long time;
};

/// @brief might be able to get rid of bright ratios, could possibly even hinder ability to recognize(?) yet to test though
struct Memory {
    std::vector<float> amplitudeRatios  = {1};
    std::vector<float> peakFreqRatios   = {1};
    std::vector<float> brightRatios     = {1};
    std::vector<float> timeRatio        = {1};
};


Memory features_to_memory(const std::vector<Feature>& features);
float cos_similarity(const std::vector<float>& a, const std::vector<float>& b);
float memory_confidence(Memory A, Memory B);

#endif