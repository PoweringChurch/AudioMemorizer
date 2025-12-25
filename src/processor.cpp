#include "processor.h"
#include "audio_consts.h"

Processor::Processor() 
    : prevAmp(0),
      cfg(kiss_fftr_alloc(FFT_SIZE, 0, NULL, NULL)),
      floatBuffer(FFT_SIZE), 
      spectrumBuffer(FFT_SIZE/2 + 1) 
{}

Processor::~Processor() {
    kiss_fftr_free(cfg);
}
void Processor::process(int16_t* samples, const int& sampleCount, const int& sampleRate) {
    update_spectrum(samples, sampleCount, floatBuffer, spectrumBuffer, cfg);

    std::array<float, 6> peaks = get_peak_freqs(sampleRate,spectrumBuffer); //points of interest
    activeChunk.push_back(peaks); //insert into active

    float amp = RMS(samples, sampleCount);
    if (abs(amp - prevAmp) > SEPERATION_THRESHOLD) {

    }
    prevAmp = amp;
}
// we dont search to see if a note exists in a song, we search to see if several notes exist seperated by a particular time.
// in practice this means that we can define a song by it's points of interest and the times between them
// aka a song really is just notes and delta times