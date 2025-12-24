#include "json.hpp"
#include "memorizer.h"
using json = nlohmann::json;

Memory features_to_memory(const std::vector<Feature>& features) {
    Memory memory = Memory();
    for (int i = 1 /* memory.ratios[0] = 1*/; i < features.size(); i++)  { 
        memory.amplitudeRatios.push_back(features[i].ampltiude/features[0].ampltiude);
        memory.peakFreqRatios.push_back(features[i].peakFrequency/features[0].peakFrequency);
        memory.brightRatios.push_back(features[i].brightness/features[0].brightness);
        memory.timeRatio.push_back(features[i].time/features[0].time);
    }
    Memory memory = Memory();
    return memory;
};

// a must be the same size as b for this to work correctly
float cos_similarity(const std::vector<float>& a, const std::vector<float>& b) {
    if (a.size() != b.size()) {
        size_t min_size = std::min(a.size(), b.size());
        std::vector<float> truncated_a(a.begin(), a.begin() + min_size);
        std::vector<float> truncated_b(b.begin(), b.begin() + min_size);
        return cos_similarity(truncated_a, truncated_b);
    }
    float dot = 0;
    float a2 = 0;
    float b2 = 0;
    for (int i = 0; i < a.size(); i++) {
        dot += a[i] * b[i];
        a2 += a[i]*a[i];
        b2 += b[i]*b[i];
    } 
    return dot/(sqrtf(a2)*sqrtf(b2));
}

float memory_confidence(Memory A, Memory B) {
    float amp_similarity = cos_similarity(A.amplitudeRatios,B.amplitudeRatios);
    float peak_freq_similarity = cos_similarity(A.peakFreqRatios,B.peakFreqRatios);
    float bright_similarity = cos_similarity(A.brightRatios,B.brightRatios);
    float time_similarity = cos_similarity(A.timeRatio,B.timeRatio);
    return (amp_similarity*AMPLITUDE_WEIGHT)
    +(peak_freq_similarity*PEAK_FREQUENCY_WEIGHT)
    +(bright_similarity*BRIGHTNESS_WEIGHT)
    +(time_similarity*TIME_WEIGHT); 
}