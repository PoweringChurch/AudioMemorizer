#ifndef ANALYZER_H
#define ANALYZER_H

#include <cstdint>
#include <cmath>
#include <complex>
#include <vector>
#include <array>
#include "../libs/kiss_fftr.h"

//tb deleted
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#include <iostream> 
#include <chrono>


class Processor;

float RMS(int16_t* samples, const int& sampleCount);
void update_spectrum(int16_t* samples, const int& sampleCount, std::vector<float>& float_buffer, std::vector<kiss_fft_cpx>& spectrum_buffer, kiss_fftr_cfg& cfg);
std::vector<float> get_peak_freqs(const int& sampleRate, const std::vector<kiss_fft_cpx>& spectrum_buffer);
float get_brightness(const int& sampleRate, const std::vector<kiss_fft_cpx>& spectrum_buffer);

//TB DELETED
float get_peak_freq_DEBUG(const int& sampleRate, const std::vector<kiss_fft_cpx>& spectrum_buffer);

#endif