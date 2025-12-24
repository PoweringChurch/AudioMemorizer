#ifndef ANALYZER_H
#define ANALYZER_H

#include <cstdint>
#include <cmath>
#include <complex>
#include <vector>
#include "../libs/kiss_fftr.h"

#include <iostream> //delete whenever out of needed

class Processor;

float RMS(int16_t* samples, const int& sampleCount);
void update_spectrum(int16_t* samples, const int& sampleCount, std::vector<float>& float_buffer, std::vector<kiss_fft_cpx>& spectrum_buffer, kiss_fftr_cfg& cfg);
float get_peak_freq(const int& sampleRate, const std::vector<kiss_fft_cpx>& spectrum_buffer);
float get_brightness(const int& sampleRate, const std::vector<kiss_fft_cpx>& spectrum_buffer);

#endif