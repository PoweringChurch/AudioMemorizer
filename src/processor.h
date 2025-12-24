#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "analyzer.h"
#include "memorizer.h"
#include <ctime>

class Processor {
public:
    Processor();
    ~Processor();
    void process(int16_t* samples, const int& sampleCount, const int& sampleRate);
private:    
    float prevAmplitude;
    kiss_fftr_cfg cfg;
    std::vector<float> floatBuffer;
    std::vector<kiss_fft_cpx> spectrumBuffer;
    std::vector<Feature> collectedFeatures;
};

#endif // PROCESSOR_H