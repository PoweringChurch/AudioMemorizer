#include "processor.h"
#include "audio_consts.h"

Processor::Processor() 
    : prevAmplitude(0),
      cfg(kiss_fftr_alloc(FFT_SIZE, 0, NULL, NULL)),
      floatBuffer(FFT_SIZE), 
      spectrumBuffer(FFT_SIZE/2 + 1) 
{}

Processor::~Processor() {
    kiss_fftr_free(cfg);
}

void Processor::process(int16_t* samples, const int& sampleCount, const int& sampleRate) {
    update_spectrum(samples, sampleCount, floatBuffer, spectrumBuffer, cfg);
    float amplitude = RMS(samples, sampleCount);
    if (abs(amplitude - prevAmplitude) > AMPLITUDE_THRESHOLD) {
        Feature poi = Feature();
        time_t time = std::time(nullptr);

        poi.ampltiude = amplitude;
        poi.peakFrequency = get_peak_freq(sampleRate, spectrumBuffer);
        poi.brightness = get_brightness(sampleRate, spectrumBuffer);
        poi.time = time;

        collectedFeatures.push_back(poi);
    } 
    prevAmplitude = amplitude;
}