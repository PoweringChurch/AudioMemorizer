#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "analyzer.h"
#include "comparator.h"



using namespace std;
class Processor {
public:
    Processor();
    ~Processor();
    //processes raw audio samples using miniaudio
    void process(int16_t* samples, const int& sampleCount, const int& sampleRate);

private:    
    /// @brief updates spectrums, for use in process
    /// @param samples 
    /// @param sampleCount 
    void update_spectrum(int16_t* samples, const int& sampleCount);
    float prevAmp; //amplitude of the previous audio sample, used for detecting points where new noise may have occured
    kiss_fftr_cfg cfg; //configuration object for kiss fft real to complex transforms
    //buffer holding audio samples converted from int16 t to float
    vector<float> floatBuffer; 
    //buffer holding complex frequency spectrum produced by the fft
    //each element represents a frequency bin with real and imaginary parts
    vector<kiss_fft_cpx> spectrumBuffer; 
    //stores the data for the currently active chunk
    vector<vector<float>> activeChunk;
    //comparator responsible for comparing extracted fingerprints against other fingerprint sets
    Comparator comparator;
};

#endif // PROCESSOR_H