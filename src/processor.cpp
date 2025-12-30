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

void Processor::update_spectrum(int16_t* samples, const int& sampleCount) {
    fill(floatBuffer.begin(), floatBuffer.end(), 0.0f); //zero out buffer
    for (int i = 0; i < sampleCount; i++) {
        float window = 0.5f * (1.0f - cos(2.0f * M_PI * i / (sampleCount - 1)));
        floatBuffer[i] = (samples[i] / 32768.0f) * window;
    }
    kiss_fftr(cfg,floatBuffer.data(),spectrumBuffer.data());
}

void Processor::process(int16_t* samples, const int& sampleCount, const int& sampleRate) {
    update_spectrum(samples, sampleCount);

    vector<float> peaks = get_peak_freqs(sampleRate,spectrumBuffer); //points of interest
    activeChunk.push_back(peaks); //insert into active

    float amp = RMS(samples, sampleCount);

    if (abs(amp - prevAmp) > SEPERATION_THRESHOLD) { //seperate
        auto start = chrono::high_resolution_clock::now();
        
        int bestMatch = comparator.find_best_match(activeChunk);
        activeChunk.clear();
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << BLUE << duration.count() << " microseconds" << RESET << endl;

        cout << "---------------------------------------" << endl;
    }
    prevAmp = amp;
}
// "we dont search to see if a note exists in a song, we search to see if several notes exist seperated by a particular time."
// in practice this means that we can define a song by it's points of interest and the times between them
// aka a song really is just notes and delta times

// "The real AI like people should have the ability to self-program and self-growth to write plug-ins for ourselves, which we call growth in our human eyes"
// kinda the philosophy going in. associate numbers with other numbers, and be able to adjust which numbers are associated with other numbers over time, provided a
// grand enough stimuli
//      -nts a few days later: got posssessed by the spirit of socrates