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

    vector<float> peaks = get_peak_freqs(sampleRate,spectrumBuffer); //points of interest
    activeChunk.push_back(peaks); //insert into active

    float amp = RMS(samples, sampleCount);

    if (abs(amp - prevAmp) > SEPERATION_THRESHOLD) { //seperate
        auto start = std::chrono::high_resolution_clock::now();
        int bestMatch = comparator.find_best_match(activeChunk);
        if (bestMatch == -1) {
            comparator.store_chunk(activeChunk);
            cout << RED << "match not found" << RESET <<endl;
        } else {
            cout << GREEN << "match found" << RESET << endl;
        }
        activeChunk.clear();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        cout << BLUE << duration.count() << " microseconds" << RESET << endl;
    }
    prevAmp = amp;
}
// "we dont search to see if a note exists in a song, we search to see if several notes exist seperated by a particular time."
// in practice this means that we can define a song by it's points of interest and the times between them
// aka a song really is just notes and delta times

// "The real AI like people should have the ability to self-program and self-growth to write plug-ins for ourselves, which we call growth in our human eyes"
// kinda the philosophy going in. associate numbers with other numbers, and be able to adjust which numbers are associated with other numbers over time, provided a
// grand enough stimuli