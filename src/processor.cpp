#include "processor.h"
#include "audio_consts.h"

Processor::Processor() 
    : prevAmplitude(0),
      cfg(kiss_fftr_alloc(FFT_SIZE, 0, NULL, NULL)),
      float_buffer(FFT_SIZE), 
      spectrum_buffer(FFT_SIZE/2 + 1) 
{}

Processor::~Processor() {
    kiss_fftr_free(cfg);
}

void Processor::process(int16_t* samples, const int& sampleCount, const int& sampleRate) {
    update_spectrum(samples, sampleCount, float_buffer, spectrum_buffer, cfg);
    float amplitude = RMS(samples, sampleCount);
    float peakFreq = get_peak_freq(sampleRate, spectrum_buffer);
    float centroid = get_centroid(sampleRate, spectrum_buffer);
    if (abs(amplitude - prevAmplitude) > AMPLITUDE_THRESHOLD) {
        std::cout << "previous rms  : " << prevAmplitude << std::endl;
        std::cout << "current rms   : " << amplitude << std::endl;
        std::cout << "diff          : " << amplitude - prevAmplitude << std::endl;
    }
    prevAmplitude = amplitude;
}