#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "analyzer.h"

class Processor {
public:
    Processor();
    ~Processor();
    void process(int16_t* samples, const int& sampleCount, const int& sampleRate);
private:    
    float prevAmp;
    kiss_fftr_cfg cfg;
    std::vector<float> floatBuffer;
    std::vector<kiss_fft_cpx> spectrumBuffer;
    std::vector<std::array<float,6>> activeChunk;
    std::vector<int> memoryHash;
};

#endif // PROCESSOR_H