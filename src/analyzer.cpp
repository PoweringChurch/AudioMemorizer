#include "analyzer.h"
#include "audio_consts.h"

float _cpx_magnitude(const kiss_fft_cpx& cpx) {
    return sqrt(cpx.r*cpx.r + cpx.i*cpx.i);
}
float RMS(int16_t* samples, const int& sampleCount) {
    float total = 0;
    for (int i = 0; i < sampleCount; i++) {
        float n = samples[i]/32768.0f;
        total += n*n;
    }
    return (float)sqrt(total/sampleCount);
}
/// @brief Calculates the peak frequencies provided the device's sample rate and a spectrum buffer
/// @param sampleRate 
/// @param spectrum_buffer 
/// @return The calculated peak frequencies
std::vector<float> get_peak_freqs(const int& sampleRate, const std::vector<kiss_fft_cpx>& spectrum_buffer) {
    std::vector<float> result;
    std::vector<float> magnitudes;
    result.reserve(6);
    magnitudes.reserve(6);
    // get up to 6 highest magnitudes out of the buffer
    for (int bin = 1; bin < spectrum_buffer.size(); bin++) {
        float magnitude = _cpx_magnitude(spectrum_buffer[bin]);
        if (magnitude < AMPLITUDE_THRESHOLD) continue;
        // find insertion point
        int insertPos = -1;
        for (int i = 0; i < magnitudes.size(); i++) {
            if (magnitude > magnitudes[i]) {
                insertPos = i;
                break;
            }
        }
        // if found a spot or haven't hit 6 yet
        if (insertPos != -1 || magnitudes.size() < 6) {
            float frequency = bin * (sampleRate / (FFT_SIZE * 2.0f));
            if (insertPos == -1 /*looks weird but the || m.size < 6 makes this valid*/) {
                magnitudes.push_back(magnitude);
                result.push_back(frequency);
            } else {
                // insert
                magnitudes.insert(magnitudes.begin() + insertPos, magnitude);
                result.insert(result.begin() + insertPos, frequency);
                
                // keep only top 6
                if (magnitudes.size() > 6) {
                    magnitudes.pop_back();
                    result.pop_back();
                }
            }
        }
    }

    return result;
}
/*
float get_brightness(const int& sampleRate, const std::vector<kiss_fft_cpx>& spectrum_buffer) {
    float w_sum = 0.0f; //weighted sum
    float m_sum = 0.0f; //magnitude sum
    for (int i = 0; i < spectrum_buffer.size(); i++) {
        float magnitude = _cpx_magnitude(spectrum_buffer[i]);
        float freq = i*(sampleRate/(float)FFT_SIZE);
        w_sum += freq*magnitude;
        m_sum += magnitude;
    }
    return w_sum/m_sum;
}
*/